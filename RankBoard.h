#pragma once
#include <string>
#include "Structs.h"

// 전방 선언
class PlayerLv;

// 티어 등급 정의
enum class RankTier {
    NONE = -1, // 초기 상태
    C, B, B_PLUS, A, A_PLUS, S, S_PLUS
};

class RankBoard {
public:
    RankBoard();
    ~RankBoard();

    // =========================================================
    // [1] 핵심 로직: GameManager에서 매 프레임/주기적 호출
    // =========================================================

    // 주간 플레이 시간 누적 (매 프레임 호출)
    void updateWeeklyTime(float dt);

    // 일일 랭크 확인 및 보상 지급 (매 프레임 또는 주기적 호출)
    // - dailySeconds: TimerSystem에서 현재 일일 시간을 받아옵니다.
    // - player: 보상을 받을 대상
    void processDailyReward(long long dailySeconds, PlayerLv& player);

    // 주간 리셋 및 보상 지급 (매 프레임 호출 - 내부에서 월요일 체크함)
    void processWeeklyReset(PlayerLv& player);

    // =========================================================
    // [2] 데이터 저장/로드용 (PlayerData 전용)
    // =========================================================
    long long getWeeklyPlayTime() const { return m_weeklyPlayTime; }
    void setWeeklyPlayTime(long long sec) { m_weeklyPlayTime = sec; }

    std::string getLastWeeklyResetDate() const { return m_lastWeeklyResetDate; }
    void setLastWeeklyResetDate(const std::string& date) { m_lastWeeklyResetDate = date; }

    RankTier getDailyHighestPaidTier() const { return m_dailyHighestPaidTier; }
    void setDailyHighestPaidTier(RankTier tier) { m_dailyHighestPaidTier = tier; }

    // =========================================================
    // [3] 정보 조회 (UI 표시용)
    // =========================================================
    std::string getTierString(RankTier tier) const;
    RankTier calculateDailyTier(long long dailySeconds) const;
    RankTier calculateWeeklyTier(long long weeklySeconds) const;

private:
    // 내부 계산 헬퍼
    float getHours(long long seconds) const;
    long long getDailyRewardXP(RankTier tier, int level) const;
    long long getWeeklyRewardXP(RankTier tier, int level) const;
    TitleBonus getWeeklyTitleBonus(RankTier tier) const;

private:
    // [랭크 시스템의 데이터 소유권은 RankBoard에 있습니다]
    long long m_weeklyPlayTime;         // 이번 주 누적 시간
    std::string m_lastWeeklyResetDate;  // 마지막으로 주간 보상을 받은 날짜 (중복 방지)
    RankTier m_dailyHighestPaidTier;    // 오늘 이미 지급받은 최고 티어 (중복 방지)
};