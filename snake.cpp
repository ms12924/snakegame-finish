#include "snake.hpp"
#include "map.hpp"

//뱀
std::deque<Point> snake; //뱀의 좌표 저장
Direction dir = RIGHT; // 현재 이동 방향 (오른쪽으로)
bool gameOver = false; // 게임오버 false로 초기화

//뱀을 맵 중간쯤에 머리 1칸, 몸통 2칸으로 초기화
void initSnake(){
    snake.push_back({10, 3}); //머리
    snake.push_back({10, 2}); //몸통
    snake.push_back({10, 1}); //몸통
    map[10][3]=3; // 3이 머리
    map[10][2]=4; // 4가 몸통
    map[10][1]=4;
}

//뱀 이동
void moveSnake(){
    Point head = snake.front();
    Point newHead = head;

    // 방향따라 새로운 머리 위치 계산
    if(dir == UP) newHead.y--;
    if(dir == DOWN) newHead.y++;
    if(dir == LEFT) newHead.x--;
    if(dir == RIGHT) newHead.x++;

    //새 머리 좌표 확인
    int cell = map[newHead.y][newHead.x];
    if (cell==1 || cell==2 || cell==4){ //머리위치가 벽, 부술 수 없는 벽, 자기 몸이면 게임오버
        gameOver=true;
        return;
    }

    //꼬리 제거
    Point tail = snake.back();
    snake.pop_back();
    map[tail.y][tail.x]=0;

    //새 머리 추가
    snake.push_front(newHead);
    map[head.y][head.x]=4; //기존 머리는 몸통으로
    map[newHead.y][newHead.x]=3; //새머리

}
