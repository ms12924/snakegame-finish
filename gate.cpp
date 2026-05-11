#include "gate.hpp"
#include <cstdlib>

extern int currentStage;

Gate gates[2];

void initGate(){
    gates[0].active=false;
    gates[1].active=false;
    spawnGate();
}

void spawnGate(){
    if(gates[0].active) gameMap[currentStage][gates[0].y][gates[0].x]=1;
    if(gates[1].active) gameMap[currentStage][gates[1].y][gates[1].x]=1;

    int y,x;
    do{
        y=rand()%MAP_SIZE;
        x=rand()%MAP_SIZE;
    }while (gameMap[currentStage][y][x] != 1);
    gates[0]={y, x, true};
    gameMap[currentStage][y][x]=7;

    do{
        y=rand()%MAP_SIZE;
        x=rand()%MAP_SIZE;
    } while (gameMap[currentStage][y][x]!=1);
    gates[1] = {y, x, true};
    gameMap[currentStage][y][x]=7;   
}

Direction getExitDir(int gateIdx, Direction enterDir){
    int y = gates[gateIdx].y;
    int x = gates[gateIdx].x;

    if(y==0) return DOWN;
    if(y==MAP_SIZE-1) return UP;
    if(x==0) return RIGHT;
    if(x==MAP_SIZE-1) return LEFT;

    Direction priority[]={enterDir,
        (Direction)((enterDir+1)%4),
        (Direction)((enterDir+3)%4),
        (Direction)((enterDir+2)%4)};
    
    for(Direction d : priority){
        int ny=y, nx=x;
        if(d==UP) ny--;
        if(d==DOWN) ny++;
        if(d==LEFT) nx--;
        if(d==RIGHT) nx++;
        if(gameMap[currentStage][ny][nx]==0) return d;
    }
    return enterDir;
}

void teleportSnake(Direction enterDir){
    Point head=snake.front();
    int exitGate=-1;
    if(head.y==gates[0].y && head.x==gates[0].x) exitGate =1;
    else exitGate=0;

    Direction exitDir = getExitDir(exitGate, enterDir);
    dir = exitDir;

    Point newHead = {gates[exitGate].y, gates[exitGate].x};
    if(exitDir==UP) newHead.y--;
    if(exitDir==DOWN) newHead.y++;
    if(exitDir==LEFT) newHead.x--;
    if(exitDir==RIGHT) newHead.x++;

    Point tail = snake.back();
    snake.pop_back();
    if(gameMap[currentStage][tail.y][tail.x]!=7){
    gameMap[currentStage][tail.y][tail.x]=0;
    }

    if(gameMap[currentStage][head.y][head.x]!=7){
    gameMap[currentStage][head.y][head.x]=4;
    }
    snake.push_front(newHead);
    gameMap[currentStage][newHead.y][newHead.x]=3;

    gameMap[currentStage][gates[0].y][gates[0].x]=7;
    gameMap[currentStage][gates[1].y][gates[1].x]=7;
}