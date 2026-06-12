#include "food.h"
#include <cstdlib>

Food::Food() {
    for (int i = 0; i < MAX_ITEMS; i++)
        items[i].timer = 0;
}

void Food::init(Board& board, int stage) {
    for (int i = 0; i < MAX_ITEMS; i++)
        items[i].timer = 0;
    spawn(board, stage);
    spawn(board, stage);
    spawn(board, stage);
}

void Food::spawn(Board& board, int stage) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].timer != 0) continue;

        int y, x;
        do {
            y = rand() % (Board::MAP_SIZE - 2) + 1;
            x = rand() % (Board::MAP_SIZE - 2) + 1;
        } while (board.getCell(stage, y, x) != 0);

        // 원래 확률 그대로: 55% 성장, 35% 독, 10% 무적
        int r = rand() % 20;
        int type = (r < 11) ? 5 : (r < 18) ? 6 : 8;

        // 무적 아이템은 동시에 1개만
        if (type == 8 && hasSpecial())
            type = (rand() % 2 == 0) ? 5 : 6;

        items[i] = {y, x, type, ITEM_DURATION};
        board.setCell(stage, y, x, type);
        break;
    }
}

void Food::update(Board& board, int stage) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].timer <= 0) continue;
        if (--items[i].timer == 0) {
            board.setCell(stage, items[i].y, items[i].x, 0);
            spawn(board, stage);
        }
    }
}

void Food::applyGrowth(Snake& snake, Board& board, int stage) {
    snake.grow(board, stage);
}

void Food::applySpecial(Snake& snake) {
    snake.setInvincible(70);
    snake.gainHeart();
}

bool Food::hasSpecial() const {
    for (int i = 0; i < MAX_ITEMS; i++)
        if (items[i].type == 8 && items[i].timer > 0) return true;
    return false;
}
