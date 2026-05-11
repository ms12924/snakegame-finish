#include "gate.hpp"
#include <cstdlib>

extern int currentStage;

Gate gates[2]; // Gate 구조체 2개를 저장

// 게이트 초기화(이전 거 비활성화) 함수
void initGate(){
    gates[0].active=false;
    gates[1].active=false;
    spawnGate();
}

// 게이트 생성 함수 
void spawnGate(){
    if(gates[0].active) gameMap[currentStage][gates[0].y][gates[0].x]=1;
    if(gates[1].active) gameMap[currentStage][gates[1].y][gates[1].x]=1;

    int y,x;
    do{ 
        y=rand()%MAP_SIZE;
        x=rand()%MAP_SIZE;
    }while (gameMap[currentStage][y][x] != 1); // 벽이 아니면 반복
    gates[0]={y, x, true};
    gameMap[currentStage][y][x]=7;

    do{
        y=rand()%MAP_SIZE;
        x=rand()%MAP_SIZE;
    } while (gameMap[currentStage][y][x]!=1);
    gates[1] = {y, x, true};
    gameMap[currentStage][y][x]=7;   
}

// 게이트 출구 방향 결정 함수
Direction getExitDir(int gateIdx, Direction enterDir){
    int y = gates[gateIdx].y;
    int x = gates[gateIdx].x;

    if(y==0) return DOWN; // 맨 위 벽이면
    if(y==MAP_SIZE-1) return UP; // 맨 아래 벽이면
    if(x==0) return RIGHT; // 맨 왼쪽 벽이면
    if(x==MAP_SIZE-1) return LEFT; // 맨 오른쪽 벽이면

    // 방향 우선순위 배열 생성
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

// 텔레포트 처리 함수 시작
void teleportSnake(Direction enterDir){
    Point head=snake.front();
    int exitGate=-1;
    if(head.y==gates[0].y && head.x==gates[0].x) exitGate =1;
    else exitGate=0;

    // 출구 방향을 계산하기
    Direction exitDir = getExitDir(exitGate, enterDir);
    dir = exitDir;

    // 새 머리 위치를 출구 게이트 좌표로 설정해 텔레포트
    Point newHead = {gates[exitGate].y, gates[exitGate].x};
    if(exitDir==UP) newHead.y--;
    if(exitDir==DOWN) newHead.y++;
    if(exitDir==LEFT) newHead.x--;
    if(exitDir==RIGHT) newHead.x++;

    Point tail = snake.back(); // 현재 꼬리를 저장
    snake.pop_back(); // 기존 꼬리 제거

    // 현재 꼬리 위치가 게이트가 아니라면
    if(gameMap[currentStage][tail.y][tail.x]!=7){
    gameMap[currentStage][tail.y][tail.x]=0; // 꼬리 위치에 빈칸
    }

    // 현재 머리 위치가 게이트가 아니라면
    if(gameMap[currentStage][head.y][head.x]!=7){
    gameMap[currentStage][head.y][head.x]=4; // 머리 위치를 몸통으로
    }
    snake.push_front(newHead); // 새 머리 추가
    gameMap[currentStage][newHead.y][newHead.x]=3;
    // 유지, 유지
    gameMap[currentStage][gates[0].y][gates[0].x]=7;
    gameMap[currentStage][gates[1].y][gates[1].x]=7;
}
