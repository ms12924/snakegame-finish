#ifndef FOOD_H
#define FOOD_H

#include "board.h"
#include "snake.h"

// 아이템 3칸 풀 전체를 관리 (type 5=성장, 6=독, 8=무적)
class Food {
public:
    static const int MAX_ITEMS    = 3;
    static const int ITEM_DURATION = 100;

    Food();
    void init(Board& board, int stage);
    void spawn(Board& board, int stage);
    void update(Board& board, int stage);
    void applyGrowth(Snake& snake, Board& board, int stage);
    void applySpecial(Snake& snake);

private:
    struct Slot { int y, x, type, timer; };
    Slot items[MAX_ITEMS];

    bool hasSpecial() const;
};

#endif
