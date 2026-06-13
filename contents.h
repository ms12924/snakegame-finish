#ifndef CONTENTS_H
#define CONTENTS_H

#include "snake.h"
#include <fstream>

class Contents {
public:
    struct Score {
        int currentLength;
        int maxLength;
        int growthItems;
        int poisonItems;
        int gateCount;
        int elapsedTime;
    };

    struct Mission {
        int targetLength;
        int targetGrowth;
        int targetPoison;
        int targetGate;
    };

    struct HighScore {
        int stage;
        int maxLength;
    };

    Contents();
    void init(const Snake& snake);
    void updateScore(const Snake& snake);
    void addGrowth();
    void addPoison();
    void addGate();
    void tickTime();
    void draw(int stage, int hearts) const;
    bool checkMission(int stage) const;
    void loadHighScore();
    void saveHighScore(int stage) const;

private:
    Score     score;
    HighScore highScore;
    static const Mission missions[5];
};

#endif
