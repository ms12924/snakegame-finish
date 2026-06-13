/*
게임 전체 흐름을 제어하는 진입점이다.
게임 객체 생성, 입력 처리, 스네이크 이동,
아이템 및 게이트 처리, 스테이지 전환,
게임 종료 조건 판정을 담당한다.
*/
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include "board.h"
#include "snake.h"
#include "food.h"
#include "poison.h"
#include "gate.h"
#include "contents.h"

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    Board    board;
    Snake    snake;
    Food     food;
    Poison   poison;
    Gate     gate;
    Contents contents;
    bool     youwin = false;
    bool     specialGateEntered = false;

    int currentStage = 0;

    /* 
    코드 블럭 로직:
    ncurses 화면 초기화,
    콘솔 화면을 게임 화면으로 사용하기 위한 설정이다.
    키 입력을 즉시 처리하고, 커서를 숨기며, 입력 대기 없이 게임 루프가 계속 실행되도록 구성한다. 
    */
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);

    /* 
    코드 블럭 로직: 
    게임에서 사용할 색상 정의,
    벽, 스네이크, 게이트, 아이템 등을 서로 다른 색상으로 출력하기 위한 설정이다. 
    */
    start_color();
    init_color(COLOR_WHITE,   700,  700,  700);
    init_color(COLOR_YELLOW, 1000, 1000,    0);
    init_color(COLOR_MAGENTA,1000,    0, 1000);
    init_color(COLOR_GREEN,     0, 1000,    0);
    init_color(COLOR_RED,    1000,    0,    0);
    init_color(COLOR_BLUE,      0,    0, 1000);
    init_color(8,            1000,  500,    0);
    init_color(9,             400,  400,  400);
    init_pair(1, COLOR_WHITE,   COLOR_WHITE);
    init_pair(2, 9,             9);
    init_pair(3, COLOR_YELLOW,  COLOR_YELLOW);
    init_pair(4, 8,             8);
    init_pair(5, COLOR_GREEN,   COLOR_GREEN);
    init_pair(6, COLOR_RED,     COLOR_RED);
    init_pair(7, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(8, COLOR_BLUE,    COLOR_BLUE);
    init_pair(9, COLOR_CYAN,    COLOR_CYAN);

    snake.init(board, currentStage);
    food.init(board, currentStage);
    gate.init(board, currentStage);
    contents.loadHighScore();
    contents.init(snake);

    int tick = 0;
    /* 
    코드 블럭 로직: 
    메인 게임 루프,
    게임 오버가 될 때까지 반복되며, 
    입력 처리 → 이동 → 충돌 판정 → 아이템 효과 적용 → 화면 갱신 순서로 동작한다. 
    */
    while (!snake.isGameOver())
    {
        int key = getch();

        /* 방향키 입력 처리,
        현재 진행 방향의 반대 방향으로는 즉시 회전할 수 없도록 제한한다. */
        Direction dir = snake.getDir();
        if (key == KEY_UP    && dir != DOWN)  snake.setDir(UP);
        if (key == KEY_DOWN  && dir != UP)    snake.setDir(DOWN);
        if (key == KEY_LEFT  && dir != RIGHT) snake.setDir(LEFT);
        if (key == KEY_RIGHT && dir != LEFT)  snake.setDir(RIGHT);

        /* 반대 방향 입력 패널티,
        본 게임에서는 반대 방향으로 진행하려할 경우 체력 감소 패널티를 부여하였다. */
        if (!snake.isInvincible() &&
            ((key == KEY_UP    && dir == DOWN)  ||
             (key == KEY_DOWN  && dir == UP)    ||
             (key == KEY_LEFT  && dir == RIGHT) ||
             (key == KEY_RIGHT && dir == LEFT))) {
            snake.loseHeart();
            if (snake.getHearts() == 0) {
                snake.setGameOver(true);
                break;
            }
        }

        tick++;
        snake.tickInvincible();

        if (tick % 10 == 0) contents.tickTime();

        /* 
        이동 및 충돌 처리,
        실제 이동 전에 다음 칸의 타입을 미리 저장하여 
        이동 이후에도 어떤 오브젝트와 충돌했는지 정확하게 판별할 수 있도록 구현하였다. */
        if (tick % 2 == 0) {
            Point     head    = snake.getHead();
            Direction curDir  = snake.getDir();
            Point     newHead = head;

            if      (curDir == UP)    newHead.y--;
            else if (curDir == DOWN)  newHead.y++;
            else if (curDir == LEFT)  newHead.x--;
            else if (curDir == RIGHT) newHead.x++;

            int headType = board.getCell(currentStage, newHead.y, newHead.x);

            snake.move(board, currentStage);

            if (headType == 5) {
                food.applyGrowth(snake, board, currentStage);
                contents.addGrowth();
            }
            if (headType == 8) {
                food.applySpecial(snake);
            }
            if (headType == 6) {
                poison.apply(snake, board, currentStage);
                contents.addPoison();
            }
            if (headType == 7) {
                gate.teleport(snake, board, currentStage);
                contents.addGate();
            }
            if (headType == 9) {
                gate.deactivateSpecial();
                specialGateEntered = true;
            }
        }
        gate.update(board, currentStage);
        gate.updateSpecial(board, currentStage, snake, currentStage == Board::STAGE_COUNT - 1);

        food.update(board, currentStage);

        contents.updateScore(snake);

        /* 
        스테이지 클리어 판정,
        미션 달성 또는 특수 게이트 진입 시 다음 스테이지로 이동한다. */
        if (contents.checkMission(currentStage) || specialGateEntered) {
            specialGateEntered = false;
            currentStage++;
            if (currentStage >= Board::STAGE_COUNT) {
                youwin = true;
                mvprintw(Board::MAP_SIZE / 2, Board::MAP_SIZE - 6, "YOU WIN!");
                refresh();
                napms(2000);
                break;
            }
            board.reset(currentStage);
            snake.init(board, currentStage);
            food.init(board, currentStage);
            gate.init(board, currentStage);
            contents.init(snake);
        }

        board.draw(currentStage, snake.isInvincible());
        contents.draw(currentStage, snake.getHearts());
        refresh();
        napms(100);
    }

    if (!youwin)
    {
        mvprintw(Board::MAP_SIZE / 2, Board::MAP_SIZE - 4, "GAME OVER");
        refresh();
        napms(2000);
    }

    contents.saveHighScore(currentStage);

    endwin();
    return 0;
}
