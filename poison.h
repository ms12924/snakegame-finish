/*
독 아이템의 효과를 관리한다.
스네이크가 독 아이템을 획득했을 때
길이 감소 또는 체력 감소 효과를 적용한다.
*/
#ifndef POISON_H
#define POISON_H

#include "board.h"
#include "snake.h"

// 독 아이템 효과 적용 
class Poison {
public:
    void apply(Snake& snake, Board& board, int stage);
};

#endif
