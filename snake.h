/**
 * @brief 뱀(Snake) 클래스 정의부
 * @details 뱀의 이동, 크기 변화, 생명 상태 제어 및 데이터 관리를 위한 인터페이스 선언
 */

#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include "board.h"

// 게임에 필요한 기본 전역 타입 정의
enum Direction { UP, DOWN, LEFT, RIGHT }; // 이동 방향 정의 (상, 하, 좌, 우)
struct Point { int y, x; };               // 2차원 좌표 표현 구조체

class Snake {
public:
    // @brief 생성자 및 초기화/제어 함수 선언
    Snake();
    void init(Board& board, int stage);                                       // 상태 초기화
    void move(Board& board, int stage);                                       // 이동 및 충돌 체크
    void grow(Board& board, int stage);                                       // 길이 연장
    void shrink(Board& board, int stage);                                     // 길이 축소
    void teleport(Point exitPos, Direction exitDir, Board& board, int stage); // 게이트 순간이동
    void setInvincible(int timer);                                            // 무적 활성화
    void tickInvincible();                                                    // 무적 시간 차감
    void setGameOver(bool val);                                               // 게임오버 플래그 설정
    void setDir(Direction d);                                                 // 이동 방향 변경
    void loseHeart();                                                         // 생명 감소
    void gainHeart();                                                         // 생명 증가

    // @brief 상태 조회용 함수 선언
    bool      isStopped()    const; // 일시정지 상태 반환
    Point     getHead()      const; // 머리 좌표 반환
    Point     getTail()      const; // 꼬리 좌표 반환
    int       size()         const; // 현재 길이 반환
    bool      isGameOver()   const; // 게임오버 여부 반환
    bool      isInvincible() const; // 무적 여부 반환
    Direction getDir()       const; // 현재 방향 반환
    int       getHearts()    const; // 잔여 생명 개수 반환

private:
    // @brief 뱀 내부 상태 데이터 관리 멤버 변수
    std::deque<Point> body;    // 뱀 몸통 좌표 목록 (머리/꼬리 확장이 유연한 deque 사용)
    Direction dir;             // 현재 진행 방향
    bool gameOver;             // 게임 종료 상태 플래그
    bool invincible;           // 무적 상태 플래그
    bool stopped;              // 충돌 후 대기(일시정지) 플래그
    int  invincibleTimer;      // 무적 지속 프레임 카운터
    int  hearts;               // 잔여 생명 카운터
};

#endif
