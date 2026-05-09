#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <deque>

enum Direction {UP, DOWN, LEFT, RIGHT};
struct Point {int y,x;};

extern std::deque<Point> snake;
extern Direction dir;
extern bool gameOver;

void initSnake();
void moveSnake();

#endif