#define MILESTONE 5  // 1=맵, 2=뱀이동, 3=아이템, 4=게이트, 5=스코어+미션

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
#if MILESTONE >= 2
    Snake    snake;
#endif
#if MILESTONE >= 3
    Food     food;
    Poison   poison;
#endif
#if MILESTONE >= 4
    Gate     gate;
#endif
#if MILESTONE >= 5
    Contents contents;
    bool     youwin = false;
#endif
#if MILESTONE >= 4
    bool specialGateEntered = false;
#endif

    int currentStage = 0;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);

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

#if MILESTONE >= 2
    snake.init(board, currentStage);
#endif
#if MILESTONE >= 3
    food.init(board, currentStage);
#endif
#if MILESTONE >= 4
    gate.init(board, currentStage);
#endif
#if MILESTONE >= 5
    contents.loadHighScore();
    contents.init(snake);
#endif

#if MILESTONE >= 2
    int tick = 0;
    while (!snake.isGameOver()) {
#else
    while (true) {
#endif
        int key = getch();

#if MILESTONE == 1
        if (key == 'q') break;
#endif

#if MILESTONE >= 2
        Direction dir = snake.getDir();
        if (key == KEY_UP    && dir != DOWN)  snake.setDir(UP);
        if (key == KEY_DOWN  && dir != UP)    snake.setDir(DOWN);
        if (key == KEY_LEFT  && dir != RIGHT) snake.setDir(LEFT);
        if (key == KEY_RIGHT && dir != LEFT)  snake.setDir(RIGHT);

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
#endif
#if MILESTONE >= 5
        if (tick % 10 == 0) contents.tickTime();
#endif

#if MILESTONE >= 2
        if (tick % 2 == 0) {
            Point     head    = snake.getHead();
            Direction curDir  = snake.getDir();
            Point     newHead = head;

            if      (curDir == UP)    newHead.y--;
            else if (curDir == DOWN)  newHead.y++;
            else if (curDir == LEFT)  newHead.x--;
            else if (curDir == RIGHT) newHead.x++;

#if MILESTONE >= 3
            int headType = board.getCell(currentStage, newHead.y, newHead.x);
#endif

            snake.move(board, currentStage);

#if MILESTONE >= 3
            if (headType == 5) {
                food.applyGrowth(snake, board, currentStage);
#if MILESTONE >= 5
                contents.addGrowth();
#endif
            }
            if (headType == 8) {
                food.applySpecial(snake);
            }
            if (headType == 6) {
                poison.apply(snake, board, currentStage);
#if MILESTONE >= 5
                contents.addPoison();
#endif
            }
#endif
#if MILESTONE >= 4
            if (headType == 7) {
                gate.teleport(snake, board, currentStage);
#if MILESTONE >= 5
                contents.addGate();
#endif
            }
            if (headType == 9) {
                gate.deactivateSpecial();
                specialGateEntered = true;
            }
#endif
        }
#endif
#if MILESTONE >= 4
        gate.updateSpecial(board, currentStage, snake, currentStage == Board::STAGE_COUNT - 1);
#endif

#if MILESTONE >= 3
        food.update(board, currentStage);
#endif
#if MILESTONE >= 5
        contents.updateScore(snake);

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
#endif

#if MILESTONE >= 2
        board.draw(currentStage, snake.isInvincible());
#else
        board.draw(currentStage, false);
#endif
#if MILESTONE >= 5
        contents.draw(currentStage, snake.getHearts());
#endif
        refresh();
        napms(100);
    }

#if MILESTONE >= 2
#if MILESTONE >= 5
    if (!youwin)
#endif
    {
        mvprintw(Board::MAP_SIZE / 2, Board::MAP_SIZE - 4, "GAME OVER");
        refresh();
        napms(2000);
    }
#endif

#if MILESTONE >= 5
    contents.saveHighScore(currentStage);
#endif

    endwin();
    return 0;
}
