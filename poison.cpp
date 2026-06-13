/*
독 아이템 획득 시 스네이크의 길이를 감소시키는 기능을 담당한다.
최소 길이 이하에서는 길이를 줄일 수 없으므로 체력을 감소시키며,
체력이 모두 소진되면 게임 오버 상태로 전환한다.
*/
#include "poison.h"

/*
코드 블럭 로직:
1. 스네이크 길이가 줄이기에 충분하지 않을 때 : 스네이크 길이가 최소 길이(3) 이하인 경우에는 체력을 감소시킨다.
                                            이때, 무적 상태가 아니어야 체력이 감소하며, 
                                            남은 체력이 없으면 게임을 종료한다.
3. 스네이크 길이가 줄이기에 충분할 때 : 길이가 충분한 경우에는 꼬리 한 칸을 제거하여 스네이크 길이를 감소시킨다.
*/
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
