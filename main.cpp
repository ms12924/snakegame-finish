#include <ncurses.h>
#include "map.hpp"
#include "snake.hpp"

int main(){
    initscr(); //ncurses 화면 초기화. 항상 맨 처음에 호출
    cbreak(); 
    noecho();
    keypad(stdscr, TRUE); //방향키같은 특수키 인식 가능
    curs_set(0);
    nodelay(stdscr, TRUE);

    start_color(); //색상 초기화
    init_color(COLOR_WHITE, 700, 700, 700);
    init_color(8, 1000, 500, 0);
    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(4, 8, 8);

    initSnake();

    int tick=0;
    while (!gameOver){
        int key=getch();

        //방향키 입력 되게
        if(key==KEY_UP && dir!=DOWN) dir=UP;
        if(key==KEY_DOWN && dir!=UP) dir=DOWN;
        if(key==KEY_LEFT && dir!=RIGHT) dir=LEFT;
        if(key==KEY_RIGHT && dir!=LEFT) dir=RIGHT;

        
        //반대 방향 입력시 게임오버
        if((key==KEY_UP && dir==DOWN) || (key==KEY_DOWN && dir==UP) || (key==KEY_LEFT && dir==RIGHT) || (key==KEY_RIGHT && dir==LEFT)){
            gameOver=true;
            break;
        }
        

        tick++;
        if(tick%5==0){ //0.5초마다 이동
            moveSnake();
        }

        drawMap();
        refresh();
        napms(100);

    }

    mvprintw(MAP_SIZE/2, MAP_SIZE-4, "GAME OVER");
    refresh();
    napms(2000);

    endwin();
    return 0;
}