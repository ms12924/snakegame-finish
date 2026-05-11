#include "item.hpp"
#include "snake.hpp"
#include <cstdlib>
#include <ctime>
extern int currentStage;

Item items[MAX_ITEMS];

//아이템 초기화
void initItems(){
    srand(time(0));
    for(int i=0; i<MAX_ITEMS; i++){
        items[i].timer=0;
    }

    spawnItem();
    spawnItem();
    spawnItem();

}

void spawnItem(){
    for(int i=0; i<MAX_ITEMS; i++){
        if(items[i].timer==0){
            int y,x;
            do{
                y=rand()%(MAP_SIZE-2)+1;
                x=rand()%(MAP_SIZE-2)+1;
            }while (gameMap[currentStage][y][x] != 0);
            items[i].y=y;
            items[i].x=x;
            items[i].type=(rand()%2==0)?5:6;
            items[i].timer=ITEM_DURATION;
            gameMap[currentStage][y][x]=items[i].type;
            break;
        }
    }
}

void updateItems(){
    for(int i=0; i<MAX_ITEMS; i++){
        if(items[i].timer>0){
            items[i].timer--;
            if(items[i].timer==0){
                gameMap[currentStage][items[i].y][items[i].x]=0;
                spawnItem();
            }
        }
    }
}

void applyItem(int type){
    if(type==5){
        Point tail=snake.back();
        snake.push_back(tail);
        gameMap[currentStage][tail.y][tail.x]=4;
    }else if(type==6){
        if(snake.size()<=3){
            gameOver=true;
            return;
        }
        Point tail=snake.back();
        snake.pop_back();
        gameMap[currentStage][tail.y][tail.x]=0;
    }
}