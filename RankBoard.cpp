#include "RankBoard.h"
#include "PlayerLv.h"
#include "DateUtils.h" // [필수] 날짜 확인용
#include <iostream>

RankBoard::RankBoard()
    : m_weeklyPlayTime(0)
    , m_lastWeeklyResetDate("")
    , m_dailyHighestPaidTier(RankTier::NONE) // 초기엔 아무것도 안 받은 상태
{
}

RankBoard::~RankBoard() {}

void RankBoard::updateWeeklyTime(float dt) {
    if (dt > 0) {
        m_weeklyPlayTime += (long long)dt;
    }
}

// [핵심] 일일 보상 지급 로직
void RankBoard::processDailyReward(long long dailySeconds, PlayerLv& player) {
    // 1. 현재 시간 기준 티어 산출
    RankTier currentTier = calculateDailyTier(dailySeconds);

    // 2. 이미 받은 티어보다 높은 등급을 달성했는지 확인
    // (Enum 값 비교: C=0, B=1 ...)
    if ((int)currentTier > (int)m_dailyHighestPaidTier) {

        // 보상 계산
        long long rewardXP = getDailyRewardXP(currentTier, player.getLevel());

        // [지급 실행] PlayerLv에게 보상을 꽂아줍니다.
        player.giveFixedXP(rewardXP);

        // [데이터 갱신] 지급 완료 처리
        m_dailyHighestPaidTier = currentTier;

        std::cout << "[Daily Rank] " << getTierString(currentTier)
            << " 달성! 보상 " << rewardXP << " XP 지급.\n";
    }
}

// [핵심] 주간 리셋 로직
void RankBoard::processWeeklyReset(PlayerLv& player) {
    std::string today = DateUtils::getToday();

    // 1. 오늘이 월요일인가?
    if (DateUtils::isMonday()) {
        // 2. 오늘 이미 리셋(보상지급)을 했는가?
        // (마지막 리셋 날짜가 오늘과 다르면 아직 안 한 것)
        if (m_lastWeeklyResetDate != today) {

            // --- 주간 정산 ---
            RankTier finalTier = calculateWeeklyTier(m_weeklyPlayTime);

            long long rewardXP = getWeeklyRewardXP(finalTier, player.getLevel());
            TitleBonus newTitle = getWeeklyTitleBonus(finalTier);

            // [지급 실행]
            player.giveFixedXP(rewardXP);   // XP 지급
            player.applyTitle(newTitle);    // 칭호 적용

            std::cout << "\n========== [WEEKLY RESET] ==========\n";
            std::cout << "주간 누적: " << getHours(m_weeklyPlayTime) << "시간\n";
            std::cout << "최종 랭크: " << getTierString(finalTier) << "\n";
            std::cout << "보상 지급: " << rewardXP << " XP, 칭호 [" << newTitle.name << "]\n";
            std::cout << "====================================\n";

            // --- 데이터 리셋 ---
            m_weeklyPlayTime = 0;          // 시간 초기화
            m_lastWeeklyResetDate = today; // 리셋 완료 도장 쾅!
        }
    }
}

// =============================================================
// 계산 공식 (요청하신 기획 반영)
// =============================================================

RankTier RankBoard::calculateDailyTier(long long dailySeconds) const {
    float hours = getHours(dailySeconds);
    if (hours >= 12.0f) return RankTier::S_PLUS;
    if (hours >= 10.0f) return RankTier::S;
    if (hours >= 8.0f)  return RankTier::A_PLUS;
    if (hours >= 6.0f)  return RankTier::A;
    if (hours >= 3.0f)  return RankTier::B_PLUS;
    if (hours >= 1.0f)  return RankTier::B;
    if (hours > 0)      return RankTier::C;
    return RankTier::NONE;
}

long long RankBoard::getDailyRewardXP(RankTier tier, int level) const {
    long long base = 0, perLv = 0;
    switch (tier) {
    case RankTier::C:      base = 500;  perLv = 50;  break;
    case RankTier::B:      base = 1000; perLv = 100; break;
    case RankTier::B_PLUS: base = 2750; perLv = 275; break;
    case RankTier::A:      base = 3500; perLv = 350; break;
    case RankTier::A_PLUS: base = 4000; perLv = 400; break;
    case RankTier::S:      base = 4500; perLv = 450; break;
    case RankTier::S_PLUS: base = 5000; perLv = 500; break;
    default: return 0;
    }
    return base + (long long)(level * perLv);
}

RankTier RankBoard::calculateWeeklyTier(long long weeklySeconds) const {
    float hours = getHours(weeklySeconds);
    if (hours >= 60.0f) return RankTier::S_PLUS;
    if (hours >= 50.0f) return RankTier::S;
    if (hours >= 45.0f) return RankTier::A_PLUS;
    if (hours >= 40.0f) return RankTier::A;
    if (hours >= 30.0f) return RankTier::B_PLUS;
    if (hours >= 20.0f) return RankTier::B;
    return RankTier::C;
}

long long RankBoard::getWeeklyRewardXP(RankTier tier, int level) const {
    long long base = 0, perLv = 0;
    switch (tier) {
    case RankTier::S_PLUS: base = 40000; perLv = 500; break;
    case RankTier::S:      base = 35000; perLv = 450; break;
    case RankTier::A_PLUS: base = 30000; perLv = 400; break;
    case RankTier::A:      base = 25000; perLv = 350; break;
    case RankTier::B_PLUS: base = 20000; perLv = 300; break;
    case RankTier::B:      base = 10000; perLv = 200; break;
    case RankTier::C:      base = 5000;  perLv = 50;  break;
    default: return 0;
    }
    return base + (long long)(level * perLv);
}

TitleBonus RankBoard::getWeeklyTitleBonus(RankTier tier) const {
    switch (tier) {
    case RankTier::S_PLUS: return TitleBonus("Legendary Artist", 2.5f, 2.0f, 3.5f);
    case RankTier::S:      return TitleBonus("Grand Master", 2.3f, 1.8f, 3.0f);
    case RankTier::A_PLUS: return TitleBonus("Master", 2.2f, 1.6f, 2.5f);
    case RankTier::A:      return TitleBonus("Professional", 1.5f, 1.4f, 2.0f);
    case RankTier::B_PLUS: return TitleBonus("Expert", 1.3f, 1.2f, 1.5f);
    case RankTier::B:      return TitleBonus("Advanced", 1.2f, 1.1f, 1.0f);
    default:               return TitleBonus("Novice", 1.0f, 1.0f, 1.0f);
    }
}

float RankBoard::getHours(long long seconds) const { return (float)seconds / 3600.0f; }

std::string RankBoard::getTierString(RankTier tier) const {
    switch (tier) {
    case RankTier::S_PLUS: return "S+";
    case RankTier::S:      return "S";
    case RankTier::A_PLUS: return "A+";
    case RankTier::A:      return "A";
    case RankTier::B_PLUS: return "B+";
    case RankTier::B:      return "B";
    case RankTier::C:      return "C";
    default:               return "-";
    }
}