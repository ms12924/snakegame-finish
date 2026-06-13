// ============================================================================
// 파일명: snake.h
// 역할: 뱀(Snake) 클래스 정의부
// 설명: 뱀의 이동, 크기 변화, 생명 상태 제어 및 데이터 관리를 위한 인터페이스 선언
// ============================================================================

#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include "board.h"

// 게임 전반에 사용되는 공통 타입 정의
enum Direction { UP, DOWN, LEFT, RIGHT }; // 이동 방향 (상, 하, 좌, 우)
struct Point { int y, x; };               // 2차원 화면 좌표 구조체

class Snake {
public:
    // 생성자 및 초기화 함수
    Snake();
    void init(Board& board, int stage);

    // 뱀의 핵심 움직임 및 상태 제어 함수
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

    // 내부 데이터 조회용 상수 함수
    bool      isStopped()    const;
    Point     getHead()      const;
    Point     getTail()      const;
    int       size()         const;
    bool      isGameOver()   const;
    bool      isInvincible() const;
    Direction getDir()       const;
    int       getHearts()    const;

private:
    // 뱀 상태 및 속성 관리용 데이터 멤버
    std::deque<Point> body;    // 뱀 몸통 좌표 목록 (양방향 확장이 유연한 deque 구조 사용)
    Direction dir;             // 현재 이동 방향
    bool gameOver;             // 게임 종료 상태 플래그
    bool invincible;           // 무적 상태 플래그
    bool stopped;              // 충돌 후 일시 정지 상태 플래그
    int  invincibleTimer;      // 무적 시간 프레임 카운터
    int  hearts;               // 잔여 목숨 카운터
};

#endif
