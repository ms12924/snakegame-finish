#include "gate.h"
#include <cstdlib>

Gate::Gate() {
    gates[0].active = false;
    gates[1].active = false;
}

void Gate::init(Board& board, int stage) {
    gates[0].active = false;
    gates[1].active = false;
    spawn(board, stage);
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
