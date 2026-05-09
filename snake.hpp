#ifndef SNAKE_HPP // snake_hpp 아직 정의 안 됐으면 아래 코드 사용
#define SNAKE_HPP

#include <deque>

enum Direction {UP, DOWN, LEFT, RIGHT}; // 뱀 이동 방향 저장
struct Point {int y,x;}; // 좌표 저장

extern std::deque<Point> snake; // 뱀 좌표 저장
extern Direction dir; // 현재 이동 방향 저장
extern bool gameOver; // 게임 종료 확인

void initSnake(); // 뱀 초기화 함수
void moveSnake(); // 뱀 이동 함수

#endif // SNAKE_HPP 끝
