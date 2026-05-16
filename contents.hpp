#ifndef CONTENTS_HPP
#define CONTENTS_HPP

struct Score{
    int currentLength;
    int maxLength;
    int growthItems;
    int poisonItems;
    int gateCount;
    int elapsedTime;
};

struct Mission{
    int targetLength;
    int targetGrowth;
    int targetPoison;
    int targetGate;
};

extern Score score;
extern Mission missions[4];

void initContents(int stage);
void updateScore();
void drawContents();
bool checkMission(int stage);

#endif
