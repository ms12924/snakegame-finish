#include "poison.h"

void Poison::apply(Snake& snake, Board& board, int stage) {
    if (snake.size() <= 3) {
        if (!snake.isInvincible()) {
            snake.loseHeart();
            if (snake.getHearts() == 0)
                snake.setGameOver(true);
        }
        return;
    }
    snake.shrink(board, stage);
}
