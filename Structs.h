#pragma once
#include <string>

// 보상 결과물 (XP, 골드 등)
struct RewardResult {
    long long xp = 0;
    long long gold = 0;
};

// 칭호 보너스 정보
struct TitleBonus {
    std::string name; // 칭호 이름
    float xpMult;     // 경험치 배율 (1.0 = 100%)
    float goldMult;   // 골드 배율
    float questMult;  // 퀘스트 보상 배율

    // 생성자 (기본값 설정)
    TitleBonus(std::string n = "None", float x = 1.0f, float g = 1.0f, float q = 1.0f)
        : name(n), xpMult(x), goldMult(g), questMult(q) {
    }
};