// ==============================================================================================================
// 파일명: contents.h
// 역할: Contens 클래스(스코어보드, 미션 관리) 선언부
// 설명: 미션 진행 상태(길이, 아이템 획득 수, 게이트 사용 수, 플레이 시간), 최고기록(단계, 길이) 저장 후 화면에 출력
// ===============================================================================================================

#ifndef CONTENTS_H
#define CONTENTS_H

#include "snake.h"
#include <fstream>

class Contents {
public:
    struct Score { // 미션 진행 상태
        int currentLength;
        int maxLength;
        int growthItems;
        int poisonItems;
        int gateCount;
        int elapsedTime;
    };

    struct Mission { // 현재 스테이지의 미션
        int targetLength;
        int targetGrowth;
        int targetPoison;
        int targetGate;
    };

    struct HighScore { // 최고기록
        int stage;
        int maxLength;
    };

    Contents();
    void init(const Snake& snake); //스테이지 시작 시 점수 초기화
    void updateScore(const Snake& snake); // 매 틱마다 현재/최대 길이 갱신
    // 아이템, 게이트, 시간 카운트
    void addGrowth();
    void addPoison();
    void addGate();
    void tickTime();

    void draw(int stage, int hearts) const; // 스코어보드, 미션, 생명 화면 출력
    bool checkMission(int stage) const; // 미션 달성 여부 확인

    // 최고기록 불러오기, 저장
    void loadHighScore();
    void saveHighScore(int stage) const;

private:
    Score     score;
    HighScore highScore;
    static const Mission missions[5]; // 스테이지별 미션 데이터
};

#endif
