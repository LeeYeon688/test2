#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h> 
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <Psapi.h>

#pragma comment(lib, "Psapi.lib") 

enum class TimerState { STOPPED, RUNNING, RESTING };

struct DailyRecord {
    std::string date;
    long long seconds;
};

class TimerSystem {
private:
    TimerState currentState;
    long long mainSessionTime;
    long long todayTotalTime;
    long long cumulativeTotalTime;
    std::chrono::steady_clock::time_point lastTick;

    // [핵심 데이터] 과거 기록을 담는 맵
    std::map<std::string, long long> m_historyMap;

    std::string getCurrentDateString();
    int getCurrentDay() const;
    bool checkPhotoshopActive();

public:
    TimerSystem();

    void clickLogin();
    void clickRest();
    void clickLogout();
    void update();

    long long getTodayTime() const { return todayTotalTime; }
    float getPureTime() const { return (float)todayTotalTime; }
    long long getTodayPureTime() const { return todayTotalTime; }
    long long getCumulativeTime() const { return cumulativeTotalTime; }
    int getTodayDateNum() const { return getCurrentDay(); }
    bool isRunning() const { return currentState == TimerState::RUNNING; }

    // [수정됨] 변수명 오타 수정 (m_history -> m_historyMap)
    long long getTotalAccumulatedTime() const {
        long long total = 0;
        // 1. 과거 기록 합산
        for (const auto& pair : m_historyMap) {
            total += pair.second;
        }
        // 2. 오늘 기록 합산 (float 변환 없이 정수 그대로 사용)
        total += todayTotalTime; 
        return total;
    }

    // PlayerData와 소통하는 함수
    const std::map<std::string, long long>& getHistoryMap() const { return m_historyMap; }
    void setHistory(const std::map<std::string, long long>& history);

    std::vector<DailyRecord> getHistory();
};