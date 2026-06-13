#ifndef BOARD_H
#define BOARD_H

class Board {
public:
    static const int MAP_SIZE = 30;
    static const int STAGE_COUNT = 5;

    Board();
    void reset(int stage);
    int  getCell(int stage, int y, int x) const;
    void setCell(int stage, int y, int x, int val);
    void draw(int stage, bool invincible) const;

private:
    int map[STAGE_COUNT][MAP_SIZE][MAP_SIZE];
    int originalMap[STAGE_COUNT][MAP_SIZE][MAP_SIZE];
};

#endif
