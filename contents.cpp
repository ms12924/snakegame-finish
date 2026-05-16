#include "contents.hpp"
#include "map.hpp"
#include "snake.hpp"
#include <ncurses.h>

extern int currentStage;

Score score;

Mission missions[4]={
    {5, 3, 2, 1},
    {7, 5, 3, 2},
    {10, 7, 4, 3},
    {13, 9, 5, 4}
};

void initContents(int stage){
    score.currentLength = snake.size();
    score.maxLength = snake.size();
    score.growthItems = 0;
    score.poisonItems = 0;
    score.gateCount = 0;
    score.elapsedTime = 0;
}

void updateScore(){
    score.currentLength = snake.size();
    if(score.currentLength > score.maxLength)
        score.maxLength = score.currentLength;
}

void drawContents(){
    int col = MAP_SIZE * 2 + 2;
    for(int i=1; i<=13; i++)
        mvprintw(i, col, "                               ");

    mvprintw(1, col, "Score Board");
    mvprintw(2, col, "B: %d / %d", score.currentLength, score.maxLength);
    mvprintw(3, col, "+: %d", score.growthItems);
    mvprintw(4, col, "-: %d", score.poisonItems);
    mvprintw(5, col, "G: %d", score.gateCount);
    mvprintw(6, col, "Time: %ds", score.elapsedTime);

    mvprintw(8, col, "Mission");
    mvprintw(9, col, "B: %d (%s)", missions[currentStage].targetLength,
        score.currentLength >= missions[currentStage].targetLength ? "v" : " ");
    mvprintw(10, col, "+: %d (%s)", missions[currentStage].targetGrowth,
        score.growthItems >= missions[currentStage].targetGrowth ? "v" : " ");
    mvprintw(11, col, "-: %d (%s)", missions[currentStage].targetPoison,
        score.poisonItems >= missions[currentStage].targetPoison ? "v" : " ");
    mvprintw(12, col, "G: %d (%s)", missions[currentStage].targetGate,
        score.gateCount >= missions[currentStage].targetGate ? "v" : " ");
}

bool checkMission(int stage){
    return score.currentLength >= missions[stage].targetLength &&
           score.growthItems >= missions[stage].targetGrowth &&
           score.poisonItems >= missions[stage].targetPoison &&
           score.gateCount >= missions[stage].targetGate;
}