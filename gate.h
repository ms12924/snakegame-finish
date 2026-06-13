/*
<Gate 클래스 선언 파일 주석 추가>

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
    // 게이트 상태 초기화
    void init(Board& board, int stage);

    // 일반 게이트 생성
    void spawn(Board& board, int stage);

    // 게이트 생성 타이머 갱신
    void update(Board& board, int stage);

    // 스네이크 게이트 순간이동 처리
    void teleport(Snake& snake, Board& board, int stage);

    // 특수 게이트 생성 및 유지 관리
    void updateSpecial(Board& board, int stage, const Snake& snake, bool isLastStage);

    // 특수 게이트 비활성화
    void deactivateSpecial();

private:
    // 게이트 위치 및 활성 상태 저장
    struct GateSlot { int y, x; bool active; };
    // 입구/출구 역할을 하는 두 개의 게이트
    GateSlot gates[2];

    // 특수 게이트 상태 정보
    bool specialActive;
    bool specialTriggered;
    int  specialY, specialX;
    int  specialTimer;

    // 일반 게이트 생성 여부 및 생성 타이머
    bool spawned;
    int  spawnTimer;

    /* 게이트 출구 방향 결정 
    1. 외곽 게이트는 맵 내부 방향으로 이동
    2. 내부 게이트는 현재 방향 → 시계 방향 → 반시계 방향 → 반대 방향 순으로 이동 가능한 방향을 탐색한다. */
    Direction getExitDir(int gateIdx, Direction enterDir,
                         const Board& board, int stage) const;
};

#endif
