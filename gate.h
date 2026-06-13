/*
게임 내 게이트(순간이동 포탈)와 특수 게이트의 상태를 관리한다.
게이트 생성, 순간이동 처리, 특수 게이트 생성 여부 및
출구 방향 결정에 필요한 정보를 저장한다.
*/
#ifndef GATE_H
#define GATE_H

#include "board.h"
#include "snake.h"

class Gate {
public:
    Gate();
    void init(Board& board, int stage);
    void spawn(Board& board, int stage);
    void update(Board& board, int stage);

    // 스네이크 게이트 순간이동 처리
    void teleport(Snake& snake, Board& board, int stage);

    // 특수 게이트 생성 및 유지 관리
    void updateSpecial(Board& board, int stage, const Snake& snake, bool isLastStage);
    void deactivateSpecial();

private:
    // 게이트 위치 및 활성 상태 저장
    struct GateSlot { int y, x; bool active; };
    GateSlot gates[2];

    bool specialActive;
    bool specialTriggered;
    int  specialY, specialX;
    int  specialTimer;

    bool spawned;
    int  spawnTimer;

    /* 게이트 출구 방향 결정: 외곽 게이트는 맵 내부 방향으로 이동시키고,  
       내부 게이트는 현재 방향 → 시계 방향 → 반시계 방향 → 반대 방향 순으로 이동 가능한 방향을 탐색한다. */
    Direction getExitDir(int gateIdx, Direction enterDir,
                         const Board& board, int stage) const;
};

#endif
