// ======================================
// 파일명 : food.h
// 역할 :  Food 클래스 선언부
// 설명 : 성장 아이템, 독 아이템, 무적 아이템 생성,
//       유지 시간, 적용 효과를 관리하는 클래스
// ======================================

#ifndef FOOD_H
#define FOOD_H

#include "board.h"
#include "snake.h"

// 성장 아이템(5), 독 아이템(6), 무적 아이템(8) 관리하는 클래스
class Food {
public:
    static const int MAX_ITEMS    = 3;
    static const int ITEM_DURATION = 100;

    Food();

    // 초기화 및 생성
    void init(Board& board, int stage);
    void spawn(Board& board, int stage);

    // 상태 업데이트
    void update(Board& board, int stage);

    // 효과 적용
    void applyGrowth(Snake& snake, Board& board, int stage);
    void applySpecial(Snake& snake);

private:
    // 아이템 한 개 위치, 종류, 시간 저장 
    struct Slot { int y, x, type, timer; };
    Slot items[MAX_ITEMS];

    bool hasSpecial() const; // 무적 아이템 존재 여부 확인
};

#endif
