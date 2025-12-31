#pragma once
#include <string>
#include <chrono>
#include <vector>
#include <map>

struct SubRecord { std::string date; long long seconds; };
enum class SubTimerState { STOPPED, RUNNING, RESTING };

class SubTimer {
public:
    SubTimer();
    void update();
    void clickLogin();
    void clickRest();
    void clickLogout();

    long long getTodayTime() const { return todayTotalTime; }
    long long getCumulativeTime() const { return cumulativeTotalTime; }
    std::vector<SubRecord> getHistory();

    // [신규] 저장된 모든 기록 + 현재 세션 시간 합산 반환
    long long getTotalAccumulatedTime() const {
        long long total = 0;
        for (const auto& [date, time] : m_historyMap) {
            total += time;
        }
        total += getTodayTime(); 
        return total;
    }

    const std::map<std::string, long long>& getHistoryMap() const { return m_historyMap; }
    
    // [수정됨] 여기 있던 중괄호 { } 본문을 삭제하고 세미콜론 ; 만 남깁니다.
    // 구현(본문)은 SubTimer.cpp에 있는 것을 사용합니다.
    void setHistory(const std::map<std::string, long long>& history);

private:
    std::string getCurrentDateString();

private:
    SubTimerState currentState;
    std::chrono::time_point<std::chrono::steady_clock> lastTick;

    std::map<std::string, long long> m_historyMap;

    long long todayTotalTime;
    long long cumulativeTotalTime;
};