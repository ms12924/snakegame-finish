#include "contents.h"
#include "board.h"
#include <ncurses.h>

const Contents::Mission Contents::missions[4] = {
    {5, 3, 2, 1},
    {7, 5, 3, 2},
    {10, 7, 4, 3},
    {13, 9, 5, 4}
};

Contents::Contents() {
    score     = {0, 0, 0, 0, 0, 0};
    highScore = {0, 0};
}

void Contents::init(const Snake& snake) {
    score.currentLength = snake.size();
    if (snake.size() > score.maxLength)
        score.maxLength = snake.size();
    score.growthItems   = 0;
    score.poisonItems   = 0;
    score.gateCount     = 0;
    score.elapsedTime   = 0;
}

void Contents::updateScore(const Snake& snake) {
    score.currentLength = snake.size();
    if (score.currentLength > score.maxLength)
        score.maxLength = score.currentLength;
}

void Contents::addGrowth() { score.growthItems++; }
void Contents::addPoison() { score.poisonItems++; }
void Contents::addGate()   { score.gateCount++; }
void Contents::tickTime()  { score.elapsedTime++; }

void Contents::draw(int stage, int hearts) const {
    const int col = Board::MAP_SIZE * 2 + 2;
    for (int i = 1; i <= 15; i++)
        mvprintw(i, col, "                               ");

    char heartBar[8] = {};
    for (int i = 0; i < 3; i++)
        heartBar[i] = (i < hearts) ? '*' : '-';
    heartBar[3] = '\0';

    mvprintw(1,  col, "Score Board  H:%s", heartBar);
    mvprintw(2,  col, "B: %d / %d", score.currentLength, score.maxLength);
    mvprintw(3,  col, "+: %d", score.growthItems);
    mvprintw(4,  col, "-: %d", score.poisonItems);
    mvprintw(5,  col, "G: %d", score.gateCount);
    mvprintw(6,  col, "Time: %ds", score.elapsedTime);

    if (highScore.stage == Board::STAGE_COUNT)
        mvprintw(8, col, "Best: CLEAR / Len %d", highScore.maxLength);
    else
        mvprintw(8, col, "Best: Stg %d / Len %d", highScore.stage + 1, highScore.maxLength);

    mvprintw(10, col, "Mission");
    mvprintw(11, col, "B: %d (%s)", missions[stage].targetLength,
        score.currentLength >= missions[stage].targetLength ? "v" : " ");
    mvprintw(12, col, "+: %d (%s)", missions[stage].targetGrowth,
        score.growthItems >= missions[stage].targetGrowth ? "v" : " ");
    mvprintw(13, col, "-: %d (%s)", missions[stage].targetPoison,
        score.poisonItems >= missions[stage].targetPoison ? "v" : " ");
    mvprintw(14, col, "G: %d (%s)", missions[stage].targetGate,
        score.gateCount >= missions[stage].targetGate ? "v" : " ");
}

void Contents::loadHighScore() {
    std::ifstream f("highscore.txt");
    if (f.is_open())
        f >> highScore.stage >> highScore.maxLength;
    else
        highScore = {0, 0};
}

void Contents::saveHighScore(int stage) const {
    bool better = (stage > highScore.stage) ||
                  (stage == highScore.stage && score.maxLength > highScore.maxLength);
    if (better) {
        std::ofstream f("highscore.txt");
        f << stage << " " << score.maxLength;
    }
}

bool Contents::checkMission(int stage) const {
    return score.currentLength >= missions[stage].targetLength &&
           score.growthItems   >= missions[stage].targetGrowth &&
           score.poisonItems   >= missions[stage].targetPoison &&
           score.gateCount     >= missions[stage].targetGate;
}
