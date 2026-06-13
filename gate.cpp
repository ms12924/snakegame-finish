// ========================================
// 파일명 : gate.cpp
// 역할 : Gate 클래스 구현부
// 설명 : 일반 게이트와 특수 게이트 생성 및 상태 관리,
//      스네이크의 순간 이동과 게이트 출구 방향 계산,
//      특수 게이트 제거를 담당.
// ========================================

#include "gate.h"
#include <cstdlib>

// 게이트 및 특수 게이트 상태 초기화
Gate::Gate() {
    gates[0].active = false;
    gates[1].active = false;
    specialActive    = false;
    specialTriggered = false;
    spawned          = false;
    spawnTimer       = 0;
}

// 스테이지 시작 시 게이트 상태, 생성 타이머 초기화
void Gate::init(Board& board, int stage) {
    gates[0].active = false;
    gates[1].active = false;
    specialActive    = false;
    spawned          = false;

    // 게이트 생성까지 대기 시간 설정
    spawnTimer       = 200; // 20초 (루프당 약 100ms)
}

// 생성 타이머 종료되면 게이트 생성
void Gate::update(Board& board, int stage) {
    if (spawned) return;
    
    if (--spawnTimer <= 0) {
        spawn(board, stage);
        spawned = true;
    }
}
/*
코드 블럭 로직:
맵의 일반 벽(=1) 중 랜덤한 두 위치를 선택하여 게이트를 생성한다.
기존 게이트가 존재하면 원래 벽 상태로 복구한 뒤 새 위치를 선정한다.
*/
void Gate::spawn(Board& board, int stage) {
    if (gates[0].active) board.setCell(stage, gates[0].y, gates[0].x, 1);
    if (gates[1].active) board.setCell(stage, gates[1].y, gates[1].x, 1);

    int y, x;
    
    // 첫 번째 게이트 생성
    do {
        y = rand() % Board::MAP_SIZE;
        x = rand() % Board::MAP_SIZE;
    } while (board.getCell(stage, y, x) != 1);
    gates[0] = {y, x, true};
    board.setCell(stage, y, x, 7);

    // 두 번째 게이트 생성
    do {
        y = rand() % Board::MAP_SIZE;
        x = rand() % Board::MAP_SIZE;
    } while (board.getCell(stage, y, x) != 1);
    gates[1] = {y, x, true};
    board.setCell(stage, y, x, 7);
}
/*
코드 블럭 로직:
게이트를 통과한 후 스네이크가 나아갈 방향을 결정한다.

1. 외곽 벽에 생성된 게이트: 맵 내부 방향으로 강제 이동
2. 내부 벽에 생성된 게이트: 현재 진행 방향, 시계 방향, 반시계 방향, 반대 방향 순서로 
   이동 가능 여부를 검사하여 가장 먼저 이동 가능한 방향을 선택한다.

이를 통해 순간이동 직후 벽에 충돌하는 상황을 방지한다.
*/
Direction Gate::getExitDir(int gateIdx, Direction enterDir,
                            const Board& board, int stage) const {
    int y = gates[gateIdx].y;
    int x = gates[gateIdx].x;
    
    // 외곽 게이트는 무조건 맵 안쪽 방향으로 이동
    if (y == 0)                    return DOWN;
    if (y == Board::MAP_SIZE - 1)  return UP;
    if (x == 0)                    return RIGHT;
    if (x == Board::MAP_SIZE - 1)  return LEFT;
    
    /* 방향 열거형 값이 순차적인 회전 구조가 아니므로 별도 테이블 사용
        즉, UP=0,DOWN=1,LEFT=2,RIGHT=3 순서라 +N으로 시계방향 계산 불가하단뜻 */
    const Direction cw[]  = {RIGHT, LEFT, UP,   DOWN}; // 시계방향
    const Direction ccw[] = {LEFT,  RIGHT, DOWN, UP  }; // 반시계방향

    Direction priority[] = {
        enterDir,
        cw[enterDir],
        ccw[enterDir],
        static_cast<Direction>(enterDir ^ 1)  // 반대방향 (UP↔DOWN, LEFT↔RIGHT)
    };
    
    // 우선순위 순서대로 이동 가능한 방향 탐색
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

/*
코드 블럭 로직:
스네이크가 게이트에 진입했을 때 수행되는 순간이동 처리.
1. 진입한 게이트를 확인
2. 반대편 게이트를 출구로 설정
3. 출구 방향 계산
4. 게이트 바로 앞 칸으로 스네이크 이동
5. 게이트 표시 복원
*/
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
    
    // 스네이크 이동 과정에서 덮어쓰인 게이트 표시 복원
    board.setCell(stage, gates[0].y, gates[0].x, 7);
    board.setCell(stage, gates[1].y, gates[1].x, 7);
}

/*
코드 블럭 로직:
특수 게이트 생성 및 유지 관리.
조건 : 체력이 1개 남은 경우에만 1회 생성/ 파란 사과가 존재하는 동안에는 생성되지 않음/
       생성 후 일정 시간이 지나면 자동으로 제거
*/
void Gate::updateSpecial(Board& board, int stage, const Snake& snake, bool isLastStage) {
    // 활성 상태일 경우 지속 시간 관리
    if (specialActive) {
        if (--specialTimer <= 0) {
            board.setCell(stage, specialY, specialX, 1);
            specialActive = false;
        }
        return;
    }

    if (isLastStage || specialTriggered) return;
    if (snake.getHearts() != 1) return; 
    
    // 무적 아이템 존재하면 특수 게이트 생성 보류
    for (int y = 0; y < Board::MAP_SIZE; y++)
        for (int x = 0; x < Board::MAP_SIZE; x++)
            if (board.getCell(stage, y, x) == 8) return; // 파란사과가 필드에 있으면 보류

    int y, x;
    // 벽 위치를 선택해 특수 게이트 생성
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

// 특수 게이트 비활성화
void Gate::deactivateSpecial() {
    specialActive = false;
}
