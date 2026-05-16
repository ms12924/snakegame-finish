#ifndef ITEM_HPP
#define ITEM_HPP

#include "map.hpp"

//랜덤 위치 생성에 필요
#include <cstdlib>
#include <ctime>

//5:growth item (뱀의 길이가 늘어남)
//6:poison item (뱀의 길이가 줄어듦)

// 아이템의 정보를 담는 구조체
struct Item{
    int y, x; // 아이템의 맵상 좌료
    int type; // 아이템 종류 (5 또는 6)
    int timer; // 아이템이 소멸되기까지 남은 시간 (Tick 단위)
};

// 시스템 설정 상수
const int MAX_ITEMS=3;  // 화면에 동시에 존재할 수 있는 최대의 아이템 수 (동시에 3개까지만 아이템 출현)
const int ITEM_DURATION=100; //아이템이 맵에 유지되는 시간 (아이템은 50틱(5초)동안 지속)

// 외부 파일에서도 접근 가능하도록 선언
extern Item items[MAX_ITEMS]; 

void initItems(); // 게임 시작 시 아이템 배열 및 시드값 초기화
void spawnItem(); // 비어 있는 슬롯에 아이템을 랜덤한 좌표(빈 공간)에 배치
void updateItems(); // 매 프레임마다 호출되어 타이머를 감소시키고, 시간이 다 된 아이템 교체
void applyItem(int type); // 뱀이 아이템을 먹었을 때 해당 타입에 맞는 효과 적용

#endif
