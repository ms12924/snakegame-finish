// ===========================================================
// 파일명 : food.cpp
// 역할 : Food 클래스 구현부
// 설명 : 성장 아이템, 독 아이템, 무적 아이템의 생성 위치와 유지 시간을 관리하고,
//       아이템을 먹었을 때 뱀에게 적용되는 효과를 처리한다.
// ===========================================================

#include "food.h"
#include <cstdlib>

// 아이템 슬롯 비워둔 상태로 초기화
Food::Food() {
    for (int i = 0; i < MAX_ITEMS; i++)
        items[i].timer = 0;
}

// 현재 스테이지에 사용할 아이템 정보 초기화하고 시작 아이템 3개 생성
void Food::init(Board& board, int stage) {
    for (int i = 0; i < MAX_ITEMS; i++)
        items[i].timer = 0;
    spawn(board, stage);
    spawn(board, stage);
    spawn(board, stage);
}

// 보드 빈 칸 찾아 성장, 독, 무적 아이템 중 하나 랜덤 생성
void Food::spawn(Board& board, int stage) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].timer != 0) continue;

        int y, x;

        // 벽, 뱀, 기존 아이템 없는 빈 공간 찾을 때까지 좌표 다시 뽑기
        do {
            y = rand() % (Board::MAP_SIZE - 2) + 1;
            x = rand() % (Board::MAP_SIZE - 2) + 1;
        } while (board.getCell(stage, y, x) != 0);

        // 성장 55%, 독 35%, 무적 10% 확률
        int r = rand() % 20;
        int type = (r < 11) ? 5 : (r < 18) ? 6 : 8;

        // 무적 아이템 여러개 동시에 안 나오게 일반 아이템으로 대체
        if (type == 8 && hasSpecial())
            type = (rand() % 2 == 0) ? 5 : 6;

        items[i] = {y, x, type, ITEM_DURATION};
        board.setCell(stage, y, x, type);
        break;
    }
}

// 아이템 남은 시간 감소, 시간 끝난 아이템은 제거 후 재생성
void Food::update(Board& board, int stage) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].timer <= 0) continue;
        if (--items[i].timer == 0) {
            board.setCell(stage, items[i].y, items[i].x, 0);
            spawn(board, stage);
        }
    }
}

// 성장 아이템 먹었을 때 뱀 길이 증가
void Food::applyGrowth(Snake& snake, Board& board, int stage) {
    snake.grow(board, stage);
}

// 무적 아이템 먹었을 때 일정 시간 무적 상태와 추가 생명 지급
void Food::applySpecial(Snake& snake) {
    snake.setInvincible(70);
    snake.gainHeart();
}

// 현재 보드에 활성화된 무적 아이템 있는지 확인
bool Food::hasSpecial() const {
    for (int i = 0; i < MAX_ITEMS; i++)
        if (items[i].type == 8 && items[i].timer > 0) return true;
    return false;
}
