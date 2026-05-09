#ifndef MAP_HPP
#define MAP_HPP

// 전역 변수 공유를 위한 extern 선언
// 다른 .cpp 파일에서도 이 변수를 사용할 수 있도록 해주는 것
extern const int MAP_SIZE; // 맵의 가로세로 크기(21)
extern const int STAGE_COUNT; // 전체 스테이지 개수 (4)

// 스테이지별 맵 데이터를 저장하는 3차원 배열
// map.cpp에 정의된 maps 배열을 참조
extern int map[4][21][21];

// 지정된 스테이지의 맵을 화면에 그리는 함수
// level: 그릴 스테이지 번호 (0 ~ STAGE_COUNT-1)
void drawMap(int level);

#endif
