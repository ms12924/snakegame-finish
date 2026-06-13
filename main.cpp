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

    snake.init(board, currentStage);
    food.init(board, currentStage);
    gate.init(board, currentStage);
    contents.loadHighScore();
    contents.init(snake);

    int tick = 0;
    while (!snake.isGameOver())
    {
        int key = getch();

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

        if (tick % 10 == 0) contents.tickTime();

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
