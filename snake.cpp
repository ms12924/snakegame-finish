// ==========================================================================================================
// 파일명: snake.cpp
// 역할: 뱀(Snake) 클래스 구현부
// 설명: 뱀의 초기화, 방향 이동 제어, 아이템 획득 시 크기 변동, 게이트 통과 처리 및 목숨/무적 상태 시스템을 구현한다.
// ==========================================================================================================

#include "snake.h"

// 생성자: 방향, 게임오버 상태, 무적 플래그, 초기 목숨(3개) 초기 설정
Snake::Snake()
    : dir(RIGHT), gameOver(false), invincible(false), stopped(false), invincibleTimer(0), hearts(3) {}

// 스테이지 시작 시 뱀 상태 및 초기 3칸 위치 세팅
void Snake::init(Board& board, int stage) {
    body.clear();
    dir             = RIGHT;
    gameOver        = false;
    invincible      = false;
    stopped         = false;
    invincibleTimer = 0;

    // 뱀 초기 위치 좌표 지정 (머리를 앞으로 하여 3칸 일렬 배치)
    body.push_back({10, 3});
    body.push_back({10, 2});
    body.push_back({10, 1});

    // 게임판 배열 내 실제 타일 값 동기화 (3: 머리, 4: 몸통)
    board.setCell(stage, 10, 3, 3);
    board.setCell(stage, 10, 2, 4);
    board.setCell(stage, 10, 1, 4);
}

// ----------------------------------------------------------------------------
// move: 매 틱마다 방향에 따라 새 좌표를 계산하고 충돌 검사 및 타일 이동을 처리
// ----------------------------------------------------------------------------
void Snake::move(Board& board, int stage) {
    if (stopped) return;
    Point head    = body.front();
    Point newHead = head;

    // 현재 설정된 방향 기준으로 다음 칸의 좌표 계산
    if      (dir == UP)    newHead.y--;
    else if (dir == DOWN)  newHead.y++;
    else if (dir == LEFT)  newHead.x--;
    else if (dir == RIGHT) newHead.x++;

    // 벽이나 자기 자신에게 충돌했는지 판단 (1: 일반 벽, 2: 모서리 벽, 4: 몸통)
    int cell = board.getCell(stage, newHead.y, newHead.x);
    if (cell == 1 || cell == 2 || cell == 4) {
        if (!invincible) {
            hearts--; // 충돌 시 목숨 감소
            if (hearts <= 0) {
                gameOver = true; // 모두 소진 시 게임 종료
            } else {
                stopped = true;  // 목숨 남았을 경우 잠시 정지 후 복귀 대기
            }
        }
        return;
    }

    // 정상 이동 로직: 한 칸 전진하므로 기존 마지막 꼬리 타일 삭제 (게이트는 보존)
    Point tail = body.back();
    body.pop_back();
    if (board.getCell(stage, tail.y, tail.x) != 7)
        board.setCell(stage, tail.y, tail.x, 0);

    // 새 머리 포인트를 push하고 기존 머리가 있던 자리는 몸통 타일(4)로 변경
    body.push_front(newHead);
    if (board.getCell(stage, head.y, head.x) != 7)
        board.setCell(stage, head.y, head.x, 4);

    board.setCell(stage, newHead.y, newHead.x, 3);
}

// 성장 아이템 획득 시 현재 꼬리 자리에 한 칸 복사 확장
void Snake::grow(Board& board, int stage) {
    Point tail = body.back();
    body.push_back(tail); 
    board.setCell(stage, tail.y, tail.x, 4);
}

// 독 아이템 획득 시 마지막 꼬리 칸 삭제 및 게임판 동기화
void Snake::shrink(Board& board, int stage) {
    Point tail = body.back();
    body.pop_back(); 
    if (board.getCell(stage, tail.y, tail.x) != 7)
        board.setCell(stage, tail.y, tail.x, 0);
}

// ----------------------------------------------------------------------------
// teleport: 게이트 통과 시 진입 꼬리/머리 잔상을 지우고 출구 좌표로 이동 처리
// ----------------------------------------------------------------------------
void Snake::teleport(Point exitPos, Direction exitDir, Board& board, int stage) {
    dir = exitDir;

    Point head = body.front();
    Point tail = body.back();
    body.pop_back();

    // 진입 전 맵에 남아있던 스네이크 타일 클리어
    if (board.getCell(stage, tail.y, tail.x) != 7)
        board.setCell(stage, tail.y, tail.x, 0);
    if (board.getCell(stage, head.y, head.x) != 7)
        board.setCell(stage, head.y, head.x, 4);

    // 설정된 출구 좌표 위치에 새 머리 타일 배치
    body.push_front(exitPos);
    board.setCell(stage, exitPos.y, exitPos.x, 3);
}

// 아이템 효과로 인한 무적 지속 시간 설정
void Snake::setInvincible(int timer) {
    invincible      = true;
    invincibleTimer = timer;
}

// 무적 활성화 상태일 때 타이머 프레임 차감 관리
void Snake::tickInvincible() {
    if (!invincible) return;
    if (--invincibleTimer <= 0)
        invincible = false;
}

// 데이터 변경 및 상태 갱신 함수
void Snake::setGameOver(bool val) { gameOver = val; }
void Snake::setDir(Direction d)   { dir = d; stopped = false; }
void Snake::loseHeart() { if (hearts > 0) hearts--; }
void Snake::gainHeart() { if (hearts < 3) hearts++; }

// 간단한 상태 조회 및 데이터 반환용 헬퍼 함수
bool Snake::isStopped() const { return stopped; }
Point      Snake::getHead()       const { return body.front(); }
Point      Snake::getTail()       const { return body.back(); }
int        Snake::size()          const { return static_cast<int>(body.size()); }
bool       Snake::isGameOver()    const { return gameOver; }
bool       Snake::isInvincible()  const { return invincible; }
Direction  Snake::getDir()        const { return dir; }
int        Snake::getHearts()     const { return hearts; }
