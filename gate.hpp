#ifndef GATE_HPP
#define GATE_HPP

#include "map.hpp"
#include "snake.hpp"

struct Gate{
    int y,x;
    bool active;
};

extern Gate gates[2];

void initGate();
void spawnGate();
Direction getExitDir(int gateIdx, Direction enterDir);
void teleportSnake(Direction enterDir);

#endif