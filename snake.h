#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include "board.h"

enum Direction { UP, DOWN, LEFT, RIGHT };
struct Point { int y, x; };

class Snake {
public:
    Snake();
    void init(Board& board, int stage);
    void move(Board& board, int stage);
    void grow(Board& board, int stage);
    void shrink(Board& board, int stage);
    void teleport(Point exitPos, Direction exitDir, Board& board, int stage);
    void setInvincible(int timer);
    void tickInvincible();
    void setGameOver(bool val);
    void setDir(Direction d);
    void loseHeart();
    void gainHeart();
    bool isStopped() const;

    Point     getHead() const;
    Point     getTail() const;
    int       size()    const;
    bool      isGameOver()   const;
    bool      isInvincible() const;
    Direction getDir()       const;
    int       getHearts()    const;

private:
    std::deque<Point> body;
    Direction dir;
    bool gameOver;
    bool invincible;
    bool stopped;
    int  invincibleTimer;
    int  hearts;
    int  pendingGrowth;
};

#endif
