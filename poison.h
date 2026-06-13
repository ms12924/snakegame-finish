// ================================================================
// 파일명: poison.h
// 역할: Poison 클래스 선언부
// 설명: 독 아이템 관리, 독 아이템 획득 시 스네이크에게 효과 적용
// ================================================================

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
