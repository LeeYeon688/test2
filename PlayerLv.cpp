#include "PlayerLv.h"
#include <cmath>
#include <iostream>

PlayerLv::PlayerLv()
    : m_totalXP(0), m_level(1)
    , m_todayTimeXP(0), m_todayTitleXP(0)
    , m_bClaimed10HrBonus(false)
    , m_bClaimed12HrBonus(false)
{
    m_currentTitle = TitleBonus("None", 1.0f, 1.0f, 1.0f);
    recalculateLevel();
}

long long PlayerLv::getRequiredXPForLevel(int lv) const {
    if (lv < 1) return 0;
    long long base = 50000;
    long long gap = 6132;
    return base + (long long)(lv - 1) * gap;
}

float PlayerLv::getTimeMultiplier(float hours) const {
    if (hours < 1.0f) return 0.5f;
    if (hours < 3.0f) return 1.0f;
    if (hours < 5.0f) return 1.2f;
    if (hours < 8.0f) return 1.5f;
    if (hours < 10.0f) return 2.0f;
    if (hours < 12.0f) return 3.0f;
    return 5.0f;
}

void PlayerLv::updateWorkExperience(float dt, float currentWorkHours) {
    if (m_level >= MAX_LEVEL) return;

    float baseXPPerSec = 100.0f / 60.0f;
    float timeMult = getTimeMultiplier(currentWorkHours);
    float titleMult = m_currentTitle.xpMult;

    // 1. 실시간 획득
    float gainTime = baseXPPerSec * timeMult * dt;
    float gainTitle = baseXPPerSec * titleMult * dt;

    if (gainTime > 0.0f)  addTimeXP((long long)(gainTime + 0.9f));
    if (gainTitle > 0.0f) addTitleXP((long long)(gainTitle + 0.9f));

    // 2. 소급 차액 계산
    const long long BASE_XP_PER_HOUR = 6000;
    long long targetTimeXP = (long long)(currentWorkHours * timeMult * BASE_XP_PER_HOUR);
    long long gap = targetTimeXP - m_todayTimeXP;

    if (gap > 100) {
        addTimeXP(gap);
    }

    // [보너스 로직]
    // 10시간 보너스
    if (currentWorkHours >= 10.0f && !m_bClaimed10HrBonus) {
        long long bonus = 40000 + (long long)(m_level * 731);
        giveFixedXP(bonus);
        m_bClaimed10HrBonus = true;
        std::cout << "\n[Bonus Reward] 10시간 돌파! 보상: " << bonus << " XP 지급.\n";
    }

    // 12시간 보너스
    if (currentWorkHours >= 12.0f && !m_bClaimed12HrBonus) {
        long long bonus = 160000 + (long long)(m_level * 1451);
        giveFixedXP(bonus);
        m_bClaimed12HrBonus = true;
        std::cout << "\n[Bonus Reward] 12시간 돌파! 보상: " << bonus << " XP 지급.\n";
    }
}

void PlayerLv::addTimeXP(long long amount) {
    m_totalXP += amount;
    m_todayTimeXP += amount;
    recalculateLevel();
}

void PlayerLv::addTitleXP(long long amount) {
    m_totalXP += amount;
    m_todayTitleXP += amount;
    recalculateLevel();
}

void PlayerLv::giveFixedXP(long long amount) {
    m_totalXP += amount;
    recalculateLevel();
}

// [추가] 링커 오류의 원인이었던 함수입니다. (칭호 적용)
void PlayerLv::applyTitle(const TitleBonus& title) {
    m_currentTitle = title;
}

void PlayerLv::setTotalXP(long long xp) {
    m_totalXP = xp;
    recalculateLevel();
}

void PlayerLv::setTitleBonus(const TitleBonus& bonus) {
    m_currentTitle = bonus;
}

void PlayerLv::recalculateLevel() {
    long long tempXP = m_totalXP;
    int calcLevel = 1;
    while (calcLevel < MAX_LEVEL) {
        long long req = getRequiredXPForLevel(calcLevel);
        if (tempXP >= req) { tempXP -= req; calcLevel++; }
        else break;
    }
    m_level = calcLevel;
    long long levelReq = getRequiredXPForLevel(m_level);
    m_currentLevelMinXP = m_totalXP - tempXP;
    m_currentLevelMaxXP = m_currentLevelMinXP + levelReq;
}

float PlayerLv::getCurrentLevelProgress() const {
    if (m_level >= MAX_LEVEL) return 1.0f;
    long long req = getRequiredXPForLevel(m_level);
    long long cur = m_totalXP - m_currentLevelMinXP;
    return (float)cur / (float)req;
}