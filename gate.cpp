/*
1. 전체 gate 클래스 요약 : 스네이크 게임의 게이트(Gate) 시스템을 관리하기 위한 클래스이다. 
    임이 시작된 후 일정 시간이 지나면 맵의 벽 위치 중 두 곳을 무작위로 선택하여 게이트를 생성하며, 
    플레이어가 한 게이트에 진입하면 다른 게이트로 이동할 수 있도록 한다.
    *전체 목표 : 게이트 생성 시점 관리, 게이트 위치 선정, 순간이동 처리, 출구 방향 계산, 특수 게이트 생성 및
                제거와 관련된 기능을 담당하며 게임의 전략성과 다양성을 높이는 역할을 수행
2. 게이트 통과 후 구체적인 스네이크 진행 방향 결정 로직 : 
    *목표 : 순간이동 직후 벽과 충돌하는 상황을 방지하고 자연스럽게 이동을 이어갈 수 있도록 함.
    2-(1) 출구 게이트가 맵의 외곽에 위치한 경우 : 스네이크가 항상 맵 내부 방향으로 이동하도록 처리한다. 
            Ex. 위쪽 벽에 생성된 게이트로 나오면 아래 방향, 왼쪽 벽에 생성된 게이트로 나오면 오른쪽 방향을 반환
    2-(2) 내부 벽에 생성된 게이트의 경우 : 현재 진행 방향을 우선 유지한다. 만약 해당 방향으로 이동할 수 없다면 
            시계 방향, 반시계 방향, 반대 방향 순으로 주변 칸을 검사하여 이동 가능한 방향을 선택한다.
            Ex. 진입 방향이 RIGHT인 경우 RIGHT → DOWN → UP → LEFT 순서로 이동 가능 여부를 확인
3. 플레이어의 생명이 1개 남은 경우 특정 조건에서 한 번만 생성되는 특수 게이트 기능 : 
    *목표 : 위기 상황의 플레이어에게 추가적인 생존 기회를 제공
        특수 게이트는 일정 시간 동안만 유지되며 시간이 만료되면 자동으로 제거된다. 
*/
#include "gate.h"
#include <cstdlib>

Gate::Gate() {
    gates[0].active = false;
    gates[1].active = false;
    specialActive    = false;
    specialTriggered = false;
    spawned          = false;
    spawnTimer       = 0;
}

void Gate::init(Board& board, int stage) {
    gates[0].active = false;
    gates[1].active = false;
    specialActive    = false;
    spawned          = false;
    spawnTimer       = 200; // 20초 (루프당 약 100ms)
}

void Gate::update(Board& board, int stage) {
    if (spawned) return;
    if (--spawnTimer <= 0) {
        spawn(board, stage);
        spawned = true;
    }
}

void Gate::spawn(Board& board, int stage) {
    if (gates[0].active) board.setCell(stage, gates[0].y, gates[0].x, 1);
    if (gates[1].active) board.setCell(stage, gates[1].y, gates[1].x, 1);

    int y, x;
    do {
        y = rand() % Board::MAP_SIZE;
        x = rand() % Board::MAP_SIZE;
    } while (board.getCell(stage, y, x) != 1);
    gates[0] = {y, x, true};
    board.setCell(stage, y, x, 7);

    do {
        y = rand() % Board::MAP_SIZE;
        x = rand() % Board::MAP_SIZE;
    } while (board.getCell(stage, y, x) != 1);
    gates[1] = {y, x, true};
    board.setCell(stage, y, x, 7);
}

Direction Gate::getExitDir(int gateIdx, Direction enterDir,
                            const Board& board, int stage) const {
    int y = gates[gateIdx].y;
    int x = gates[gateIdx].x;

    if (y == 0)                    return DOWN;
    if (y == Board::MAP_SIZE - 1)  return UP;
    if (x == 0)                    return RIGHT;
    if (x == Board::MAP_SIZE - 1)  return LEFT;

    // UP=0,DOWN=1,LEFT=2,RIGHT=3 순서라 +N으로 시계방향 계산 불가 → 테이블 사용
    const Direction cw[]  = {RIGHT, LEFT, UP,   DOWN}; // 시계방향
    const Direction ccw[] = {LEFT,  RIGHT, DOWN, UP  }; // 역시계방향

    Direction priority[] = {
        enterDir,
        cw[enterDir],
        ccw[enterDir],
        static_cast<Direction>(enterDir ^ 1)  // 반대방향 (UP↔DOWN, LEFT↔RIGHT)
    };

    for (Direction d : priority) {
        int ny = y, nx = x;
        if (d == UP)    ny--;
        if (d == DOWN)  ny++;
        if (d == LEFT)  nx--;
        if (d == RIGHT) nx++;
        if (board.getCell(stage, ny, nx) == 0) return d;
    }
    return enterDir;
}

void Gate::teleport(Snake& snake, Board& board, int stage) {
    Point head = snake.getHead();

    int exitGate = (head.y == gates[0].y && head.x == gates[0].x) ? 1 : 0;

    Direction exitDir = getExitDir(exitGate, snake.getDir(), board, stage);

    Point exitPos = {gates[exitGate].y, gates[exitGate].x};
    if (exitDir == UP)    exitPos.y--;
    if (exitDir == DOWN)  exitPos.y++;
    if (exitDir == LEFT)  exitPos.x--;
    if (exitDir == RIGHT) exitPos.x++;

    snake.teleport(exitPos, exitDir, board, stage);

    board.setCell(stage, gates[0].y, gates[0].x, 7);
    board.setCell(stage, gates[1].y, gates[1].x, 7);
}

void Gate::updateSpecial(Board& board, int stage, const Snake& snake, bool isLastStage) {
    if (specialActive) {
        if (--specialTimer <= 0) {
            board.setCell(stage, specialY, specialX, 1);
            specialActive = false;
        }
        return;
    }

    if (isLastStage || specialTriggered) return;
    if (snake.getHearts() != 1) return;

    for (int y = 0; y < Board::MAP_SIZE; y++)
        for (int x = 0; x < Board::MAP_SIZE; x++)
            if (board.getCell(stage, y, x) == 8) return; // 파란사과가 필드에 있으면 보류

    int y, x;
    do {
        y = rand() % Board::MAP_SIZE;
        x = rand() % Board::MAP_SIZE;
    } while (board.getCell(stage, y, x) != 1);

    specialY = y;
    specialX = x;
    board.setCell(stage, y, x, 9);
    specialActive    = true;
    specialTriggered = true;
    specialTimer     = 100; // 10초 (루프당 약 100ms)
}

void Gate::deactivateSpecial() {
    specialActive = false;
}
