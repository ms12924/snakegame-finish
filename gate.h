#ifndef GATE_H
#define GATE_H

#include "board.h"
#include "snake.h"

class Gate {
public:
    Gate();
    void init(Board& board, int stage);
    void spawn(Board& board, int stage);
    void teleport(Snake& snake, Board& board, int stage);

    void updateSpecial(Board& board, int stage, const Snake& snake, bool isLastStage);
    void deactivateSpecial();

private:
    struct GateSlot { int y, x; bool active; };
    GateSlot gates[2];

    bool specialActive;
    bool specialTriggered;
    int  specialY, specialX;
    int  specialTimer;

    Direction getExitDir(int gateIdx, Direction enterDir,
                         const Board& board, int stage) const;
};

#endif
