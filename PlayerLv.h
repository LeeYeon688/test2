#pragma once

// [필수] 인터페이스 헤더 포함
#include "PlayerStatus.h" 
#include "Structs.h" 
#include <iostream>

// [변경] PlayerStatus를 상속받습니다.
class PlayerLv : public PlayerStatus {
public:
    PlayerLv();

    // =========================================================
    // [1] PlayerStatus 인터페이스 구현 (Override)
    // =========================================================
    // SystemManager가 이 함수들을 통해 상태를 확인합니다.
    int getLevel() const override { return m_level; }
    long long getTotalXP() const override { return m_totalXP; }
    float getCurrentLevelProgress() const override;

    // =========================================================
    // [2] 핵심 로직 (GameManager가 호출)
    // =========================================================

    // 1. 노동(시간) 경험치 업데이트 (소급 적용 대상)
    // dt: 실시간 흐름, currentWorkHours: 총 누적 시간
    void updateWorkExperience(float dt, float currentWorkHours);

    // 2. 칭호/퀘스트 등 비노동 경험치 지급 (소급 제외)
    // 노동 장부에 기록하지 않는 순수 보너스입니다.
    void giveFixedXP(long long amount);

    // 3. 칭호 적용 (RankBoard가 이 함수를 호출해 칭호를 바꿉니다)
    void applyTitle(const TitleBonus& title);

    // =========================================================
    // [3] 데이터 로드/저장용 (PlayerData가 호출)
    // =========================================================
    void setTotalXP(long long xp);
    void setTitleBonus(const TitleBonus& bonus);
    const TitleBonus& getCurrentTitle() const { return m_currentTitle; }

    // [장부 1] 순수 시간 노동 장부 (소급 계산용)
    long long getTodayTimeXP() const { return m_todayTimeXP; }
    void setTodayTimeXP(long long xp) { m_todayTimeXP = xp; }

    // [장부 2] 칭호 보너스 장부 (독립 기록용)
    long long getTodayTitleXP() const { return m_todayTitleXP; }
    void setTodayTitleXP(long long xp) { m_todayTitleXP = xp; }
    // =========================================================
    // [신규] 랭크 보상 지급 여부 확인용 (중복 방지)
    // =========================================================
    bool hasClaimed10HrBonus() const { return m_bClaimed10HrBonus; }
    bool hasClaimed12HrBonus() const { return m_bClaimed12HrBonus; }

    void setClaimed10HrBonus(bool claimed) { m_bClaimed10HrBonus = claimed; }
    void setClaimed12HrBonus(bool claimed) { m_bClaimed12HrBonus = claimed; }


private:
    void recalculateLevel();
    long long getRequiredXPForLevel(int level) const;
    float getTimeMultiplier(float hours) const;

    // 내부 장부 기록용 함수
    void addTimeXP(long long amount);
    void addTitleXP(long long amount);

private:
    long long m_totalXP;
    int m_level;

    // [장부 분리: 소급 차액 계산의 정확성을 위함]
    long long m_todayTimeXP;  // 순수 시간 배율로 번 XP (소급 대상)
    long long m_todayTitleXP; // 칭호 보너스로 번 XP (소급 X, 독립)

    // [수정] 변수명 변경 (S랭크 -> 10시간 보너스)
    bool m_bClaimed10HrBonus; // 10시간 보상 받았니?
    bool m_bClaimed12HrBonus; // 12시간 보상 받았니?

    long long m_currentLevelMinXP;
    long long m_currentLevelMaxXP;
    const int MAX_LEVEL = 100;

    TitleBonus m_currentTitle;
};