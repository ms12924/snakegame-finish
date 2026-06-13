// ============================================================================
// 파일명: board.h
// 역할: 게임판(Board) 클래스 정의부
// 설명: 스테이지별 맵 크기 상수 정의 및 맵 데이터 관리용 멤버 함수/변수 선언
// ============================================================================

#ifndef BOARD_H
#define BOARD_H

class Board {
public:
    // 게임판 관련 고정 상수 설정
    static const int MAP_SIZE = 30;     // 맵 가로세로 크기 (30x30 고정)
    static const int STAGE_COUNT = 5;   // 총 스테이지 개수 (1~5 스테이지)

    // 생성자 및 멤버 함수 정의
    Board();
    void reset(int stage);                                   // 맵 상태 초기화
    int  getCell(int stage, int y, int x) const;             // 특정 좌표 타일 상태 반환
    void setCell(int stage, int y, int x, int val);          // 특정 좌표 타일 값 변경
    void draw(int stage, bool invincible) const;             // 현재 스테이지 화면 렌더링

private:
    // 스테이지별 맵 데이터를 저장하는 3차원 배열 [스테이지 번호][Y축][X축]
    int map[STAGE_COUNT][MAP_SIZE][MAP_SIZE];         // 실시간 게임 진행용 변동 맵
    int originalMap[STAGE_COUNT][MAP_SIZE][MAP_SIZE]; // 초기 상태 복구용 백업 맵
};

#endif
