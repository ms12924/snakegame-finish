#include "item.hpp"
#include "snake.hpp"
#include <cstdlib>
#include <ctime>
extern int currentStage;
extern bool invincible;
extern int invincibleTimer;

Item items[MAX_ITEMS]; // 필드에 생성될 아이템 관리 배열

// 게임 시작 시 아이템 상태 초기화 및 최초 생성
void initItems(){
    srand(time(0)); // 랜덤 시드 설정
    for(int i=0; i<MAX_ITEMS; i++){
        items[i].timer=0; // 모든 아이템 슬롯 비활성화
    }

    // 초기 아이템 3개 배치
    spawnItem();
    spawnItem();
    spawnItem();

}

// 비어 있는 아이템 슬롯에 새로운 아이템을 맵의 빈 공간에 배치
void spawnItem(){
    for(int i=0; i<MAX_ITEMS; i++){
        if(items[i].timer==0){ // 사용 가능한 슬롯 찾기
            int y,x;
            do{
                // 벽 (0번 외)이나 뱀의 몸통이 없는 위치를 찾을 때까지 반복
                y=rand()%(MAP_SIZE-2)+1;
                x=rand()%(MAP_SIZE-2)+1;
            }while (gameMap[currentStage][y][x] != 0);
            items[i].y=y;
            items[i].x=x;
            // 5: Growth Item(몸 길이 증가), 6: Poison Item(몸 길이 감소)
            int r = rand()%20;
            int newType=(r<11)?5:(r<18)?6:8;

            if(newType==8){
                for(int j=0; j<MAX_ITEMS; j++){
                    if(items[j].type==8 && items[j].timer >0){
                        newType=(rand()%2==0) ? 5:6;
                        break;
                    }
                }
            }
            items[i].type=newType;
            items[i].timer=ITEM_DURATION; // 아이템 유지 시간 설정
            gameMap[currentStage][y][x]=items[i].type; // 맵 데이터 업데이트
            break;
        }
    }
}

// 매 프레임마다 호출하여 아이템의 남은 시간을 갱신
void updateItems(){
    for(int i=0; i<MAX_ITEMS; i++){
        if(items[i].timer>0){
            items[i].timer--; // 타이머 감소

            // 제한 시간이 다 되면 아이템을 제거하고 새로운 위치에 생성
            if(items[i].timer==0){
                gameMap[currentStage][items[i].y][items[i].x]=0; // 맵에서 지우기
                spawnItem();
            }
        }
    }
}

// 뱀이 아이템을 먹었을 때 종류에 따른 효과 적용
void applyItem(int type){
    if(type==5){ // Growth Item: 꼬리 쪽에 마디 추가
        Point tail=snake.back();
        snake.push_back(tail);
        gameMap[currentStage][tail.y][tail.x]=4; // 4: 뱀 몸통 번호
    }else if(type==6){ // Posion Item: 몸 길이 감소
        // 뱀의 길이가 3 이하일 때 독약을 먹으면 게임 오버
        if(snake.size()<=3){
            if(!invincible){
                gameOver=true;            }
            return;
        }
        Point tail=snake.back();
        snake.pop_back(); // 기존 꼬리 위치 비우기
        gameMap[currentStage][tail.y][tail.x]=0; // 뱀 마디 제
    }else if(type==8){
        invincible=true;
        invincibleTimer=100;
    }
}
