/**
 * @brief 뱀(Snake) 클래스 구현부
 * @details 뱀의 초기화, 이동(move), 성장/축소, 게이트 순간이동 및 무적/생명 상태 관리
 */

#include "snake.h"

/**
 * @brief Snake 클래스 생성자
 * @details 초기 방향, 게임오버 상태, 무적 타이머, 초기 목숨(3개) 초기화
 */
Snake::Snake()
    : dir(RIGHT), gameOver(false), invincible(false), stopped(false), invincibleTimer(0), hearts(3) {}

/**
 * @brief 스테이지 시작 시 뱀 상태 및 위치 초기화
 * @param board Board 클래스 참조 변수
 * @param stage 현재 진행할 스테이지 번호
 */
void Snake::init(Board& board, int stage) {
    body.clear();
    dir             = RIGHT;
    gameOver        = false;
    invincible      = false;
    stopped         = false;
    invincibleTimer = 0;

    // 뱀 초기 3칸 위치 설정 (X축 기준 3->2->1 순서로 머리->몸통 배치)
    body.push_back({10, 3});
    body.push_back({10, 2});
    body.push_back({10, 1});

    // Board 객체 맵 내 타일 값 동기화 (3: 머리, 4: 몸통)
    board.setCell(stage, 10, 3, 3);
    board.setCell(stage, 10, 2, 4);
    board.setCell(stage, 10, 1, 4);
}

/**
 * @brief 입력 방향 기반 뱀 이동 및 충돌 처리
 * @param board Board 클래스 참조 변수
 * @param stage 현재 스테이지 번호
 */
void Snake::move(Board& board, int stage) {
    if (stopped) return;
    Point head    = body.front();
    Point newHead = head;

    // 현재 방향 기준 다음 이동 좌표 계산
    if      (dir == UP)    newHead.y--;
    else if (dir == DOWN)  newHead.y++;
    else if (dir == LEFT)  newHead.x--;
    else if (dir == RIGHT) newHead.x++;

    // 충돌 타일 체크 (1: 일반 벽, 2: 모서리 벽, 4: 자신의 몸통)
    int cell = board.getCell(stage, newHead.y, newHead.x);
    if (cell == 1 || cell == 2 || cell == 4) {
        if (!invincible) {
            hearts--; // [독창적 규칙 가산점] 생명 감소 처리
            if (hearts <= 0) {
                gameOver = true; // 생명 모두 소진 시 게임오버
            } else {
                stopped = true;  // 생명 남았을 경우 일시 정지 후 대기
            }
        }
        return;
    }

    // 정상 이동 처리: 기존 꼬리 타일 제거 (게이트 타일 제외)
    Point tail = body.back();
    body.pop_back();
    if (board.getCell(stage, tail.y, tail.x) != 7)
        board.setCell(stage, tail.y, tail.x, 0);

    // 새 머리 배치 및 기존 머리 타일을 몸통으로 전환
    body.push_front(newHead);
    if (board.getCell(stage, head.y, head.x) != 7)
        board.setCell(stage, head.y, head.x, 4);

    board.setCell(stage, newHead.y, newHead.x, 3);
}

// @brief 성장 아이템 획득 시 뱀 길이 증가
void Snake::grow(Board& board, int stage) {
    Point tail = body.back();
    body.push_back(tail); // 꼬리 위치에 몸통 한 칸 복사 확장
    board.setCell(stage, tail.y, tail.x, 4);
}

// @brief 독 아이템 획득 시 뱀 길이 축소
void Snake::shrink(Board& board, int stage) {
    Point tail = body.back();
    body.pop_back(); // 마지막 꼬리 칸 삭제
    if (board.getCell(stage, tail.y, tail.x) != 7)
        board.setCell(stage, tail.y, tail.x, 0);
}

/**
 * @brief 게이트 진입 시 출구 위치로 뱀 좌표 순간이동
 * @param exitPos 출구 좌표
 * @param exitDir 출구 진출 방향
 */
void Snake::teleport(Point exitPos, Direction exitDir, Board& board, int stage) {
    dir = exitDir;

    Point head = body.front();
    Point tail = body.back();
    body.pop_back();

    // 기존 꼬리 및 머리 타일 맵 업데이트
    if (board.getCell(stage, tail.y, tail.x) != 7)
        board.setCell(stage, tail.y, tail.x, 0);
    if (board.getCell(stage, head.y, head.x) != 7)
        board.setCell(stage, head.y, head.x, 4);

    // 출구 위치에 새 머리 배치
    body.push_front(exitPos);
    board.setCell(stage, exitPos.y, exitPos.x, 3);
}

/**
 * @brief 무적 상태 활성화 및 타이머 세팅
 * @param timer 무적 지속 시간 프레임 수
 */
void Snake::setInvincible(int timer) {
    invincible      = true;
    invincibleTimer = timer;
}

// @brief 무적 시간 프레임 차감 및 타이머 만료 체크
void Snake::tickInvincible() {
    if (!invincible) return;
    if (--invincibleTimer <= 0)
        invincible = false;
}

// 상태 설정 및 제어 함수 인터페이스 구현부
void Snake::setGameOver(bool val) { gameOver = val; }
void Snake::setDir(Direction d)   { dir = d; stopped = false; }
void Snake::loseHeart() { if (hearts > 0) hearts--; }
void Snake::gainHeart() { if (hearts < 3) hearts++; }

bool Snake::isStopped() const { return stopped; }
Point      Snake::getHead()       const { return body.front(); }
Point      Snake::getTail()       const { return body.back(); }
int        Snake::size()          const { return static_cast<int>(body.size()); }
bool       Snake::isGameOver()    const { return gameOver; }
bool       Snake::isInvincible()  const { return invincible; }
Direction  Snake::getDir()        const { return dir; }
int        Snake::getHearts()     const { return hearts; }
