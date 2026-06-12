#ifndef POISON_H
#define POISON_H

#include "board.h"
#include "snake.h"

// 독 아이템 효과 적용 담당
class Poison {
public:
    void apply(Snake& snake, Board& board, int stage);
};

#endif
