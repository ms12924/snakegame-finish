#include <ncurses.h> // ncurses 기능 사용
#include "map.hpp" // 맵 출력 함수
#include "snake.hpp" // 뱀 함수
#include "item.hpp"
#include "gate.hpp"
#include "contents.hpp"

int currentStage=0;
bool youwin = false;

int main(){
    initOriginalMap();
    initscr(); // ncurses 화면 초기화. 항상 맨 처음에 호출
    cbreak(); 
    noecho();
    keypad(stdscr, TRUE); // 방향키같은 특수키 인식 가능
    curs_set(0);
    nodelay(stdscr, TRUE); // 프로그램 멈추지 않도록 함

    start_color(); //색상 초기화
    init_color(COLOR_WHITE, 700, 700, 700);
    init_color(COLOR_YELLOW, 1000, 1000, 0);
    init_color(COLOR_MAGENTA, 1000, 0, 1000);
    init_color(COLOR_GREEN, 0, 1000, 0);
    init_color(COLOR_RED, 1000, 0, 0);
    init_color(COLOR_BLUE, 0, 0, 1000);
    init_color(8, 1000, 500, 0);
    init_color(9, 400, 400, 400);
    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    init_pair(2, 9, 9);
    init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(4, 8, 8);
    init_pair(5, COLOR_GREEN, COLOR_GREEN);
    init_pair(6, COLOR_RED, COLOR_RED);
    init_pair(7, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(8, COLOR_BLUE, COLOR_BLUE);

    initSnake();
    initItems();
    initGate();
    initContents(currentStage);

    int tick=0;
    while (!gameOver){
        int key=getch();

        //방향키 입력 되도록
        if(key==KEY_UP && dir!=DOWN) dir=UP;
        if(key==KEY_DOWN && dir!=UP) dir=DOWN;
        if(key==KEY_LEFT && dir!=RIGHT) dir=LEFT;
        if(key==KEY_RIGHT && dir!=LEFT) dir=RIGHT;

        
        //반대 방향 입력시 게임오버 되도록
        if((key==KEY_UP && dir==DOWN) || (key==KEY_DOWN && dir==UP) || (key==KEY_LEFT && dir==RIGHT) || (key==KEY_RIGHT && dir==LEFT)){
            if(!invincible){
            gameOver=true;
            break;
            }
        }
        

        tick++;
        if(invincible){
            invincibleTimer--;
            if(invincibleTimer<=0) invincible=false;
        }
        if(tick % 10 ==0) score.elapsedTime++;
        if(tick%3==0){ //0.5초마다 이동
            Point head = snake.front();
            Point newHead = head;
            if(dir==UP) newHead.y--;
            if(dir==DOWN) newHead.y++;
            if(dir==LEFT) newHead.x--;
            if(dir==RIGHT) newHead.x++;
            int headType=gameMap[currentStage][newHead.y][newHead.x];

            moveSnake();

            if(headType==5 || headType==6){
                applyItem(headType);
                if(headType==5) score.growthItems++;
                if(headType==6) score.poisonItems++;
            }
            if(headType==7){
                teleportSnake(dir);
                score.gateCount++;
            }
            if(headType==8){
                applyItem(headType);
            }
        }
        updateItems();
        updateScore();

        if(checkMission(currentStage)){
            currentStage++;
            youwin=true;
            if(currentStage >= STAGE_COUNT){
                mvprintw(MAP_SIZE/2, MAP_SIZE-6, "YOU WIN!");
                refresh();
                napms(2000);
                break;
            }
            
            resetMap(currentStage);
            initSnake();
            initItems();
            initGate();
            initContents(currentStage);
        }

        drawMap(currentStage); // 현재 맵 상태
        drawContents();
        refresh(); // 화면에 반영
        napms(100); //0.1초 대기

    }

    // 게임오버가 되었을 때
    if(!youwin){
    mvprintw(MAP_SIZE/2, MAP_SIZE-4, "GAME OVER");
    refresh(); 
    napms(2000); // 게임오버 화면 2초 동안 유지
    }

    endwin();
    return 0; // 프로그램 종료
}