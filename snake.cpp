#include "snake.h"

Snake::Snake()
    : dir(RIGHT), gameOver(false), invincible(false), stopped(false), invincibleTimer(0), hearts(3) {}

void Snake::init(Board& board, int stage) {
    body.clear();
    dir             = RIGHT;
    gameOver        = false;
    invincible      = false;
    stopped         = false;
    invincibleTimer = 0;
    hearts          = 3;

    body.push_back({10, 3});
    body.push_back({10, 2});
    body.push_back({10, 1});

    board.setCell(stage, 10, 3, 3);
    board.setCell(stage, 10, 2, 4);
    board.setCell(stage, 10, 1, 4);
}

void Snake::move(Board& board, int stage) {
    if (stopped) return;
    Point head    = body.front();
    Point newHead = head;

    if      (dir == UP)    newHead.y--;
    else if (dir == DOWN)  newHead.y++;
    else if (dir == LEFT)  newHead.x--;
    else if (dir == RIGHT) newHead.x++;

    int cell = board.getCell(stage, newHead.y, newHead.x);
    if (cell == 1 || cell == 2 || cell == 4) {
        if (!invincible) {
            hearts--;
            if (hearts <= 0) {
                gameOver = true;
            } else {
                stopped = true;
            }
        }
        return;
    }

    Point tail = body.back();
    body.pop_back();
    if (board.getCell(stage, tail.y, tail.x) != 7)
        board.setCell(stage, tail.y, tail.x, 0);

    body.push_front(newHead);
    if (board.getCell(stage, head.y, head.x) != 7)
        board.setCell(stage, head.y, head.x, 4);

    board.setCell(stage, newHead.y, newHead.x, 3);
}

void Snake::grow(Board& board, int stage) {
    Point tail = body.back();
    body.push_back(tail);
    board.setCell(stage, tail.y, tail.x, 4);
}

void Snake::shrink(Board& board, int stage) {
    Point tail = body.back();
    body.pop_back();
    if (board.getCell(stage, tail.y, tail.x) != 7)
        board.setCell(stage, tail.y, tail.x, 0);
}

void Snake::teleport(Point exitPos, Direction exitDir, Board& board, int stage) {
    dir = exitDir;

    Point head = body.front();
    Point tail = body.back();
    body.pop_back();

    if (board.getCell(stage, tail.y, tail.x) != 7)
        board.setCell(stage, tail.y, tail.x, 0);
    if (board.getCell(stage, head.y, head.x) != 7)
        board.setCell(stage, head.y, head.x, 4);

    body.push_front(exitPos);
    board.setCell(stage, exitPos.y, exitPos.x, 3);
}

void Snake::setInvincible(int timer) {
    invincible      = true;
    invincibleTimer = timer;
}

void Snake::tickInvincible() {
    if (!invincible) return;
    if (--invincibleTimer <= 0)
        invincible = false;
}

void Snake::setGameOver(bool val) { gameOver = val; }
void Snake::setDir(Direction d)   { dir = d; stopped = false; }
void Snake::loseHeart() { if (hearts > 0) hearts--; }
void Snake::gainHeart() { if (hearts < 3) hearts++; }
bool Snake::isStopped() const { return stopped; }

Point     Snake::getHead()      const { return body.front(); }
Point     Snake::getTail()      const { return body.back(); }
int       Snake::size()         const { return static_cast<int>(body.size()); }
bool      Snake::isGameOver()   const { return gameOver; }
bool      Snake::isInvincible() const { return invincible; }
Direction Snake::getDir()       const { return dir; }
int       Snake::getHearts()    const { return hearts; }
