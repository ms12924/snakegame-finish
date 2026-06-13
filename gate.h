// =======================================
// 파일명 : gate.h
// 역할 : Gate 클래스 선언부
// 설명 : 일반 게이트와 특수 게이트 생성 및 상태 관리,
//       뱀의 순간이동과 게이트 출구 방향 결정 담당
// =======================================

#ifndef GATE_H
#define GATE_H

#include "board.h"
#include "snake.h"

// 게이트 생성, 순간이동 및 특수 게이트 관리를 담당
class Gate {
public:
    Gate();

    // 초기화 및 생성
    void init(Board& board, int stage);
    void spawn(Board& board, int stage);

    // 상태 업데이트
    void update(Board& board, int stage);

    // 스네이크 게이트 진입시 순간이동 처리
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
