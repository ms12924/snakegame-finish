/**
 * @brief 게임판(Board) 클래스 정의부
 * @details 3차원 배열 기반 스테이지별 맵 데이터 저장 및 관리용 멤버 선언
 */

#ifndef BOARD_H
#define BOARD_H

class Board {
public:
    // [지침 준수] 변경되지 않는 변수에 대한 상수(const) 명시 및 static 선언
    static const int MAP_SIZE = 30;     // 맵 가로세로 크기 (30x30 고정)
    static const int STAGE_COUNT = 5;   // 총 스테이지 개수 (1~5 스테이지)

    // @brief 생성자 및 멤버 함수 선언
    Board();
    void reset(int stage);                                   // 맵 상태 초기화
    int  getCell(int stage, int y, int x) const;             // 특정 좌표 타일 반환 (const 상수 함수)
    void setCell(int stage, int y, int x, int val);          // 특정 좌표 타일 설정
    void draw(int stage, bool invincible) const;             // 맵 화면 렌더링 (const 상수 함수)

private:
    // @brief 맵 상태 저장용 3차원 배열 [스테이지][Y축][X축]
    int map[STAGE_COUNT][MAP_SIZE][MAP_SIZE];         // 실시간 게임 진행용 변동 맵
    int originalMap[STAGE_COUNT][MAP_SIZE][MAP_SIZE]; // 초기 상태 복구용 백업 맵
};

#endif
