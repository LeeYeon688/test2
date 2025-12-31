#include "SubTimer.h"
#include <ctime>

SubTimer::SubTimer() {
    currentState = SubTimerState::STOPPED;
    todayTotalTime = 0;
    cumulativeTotalTime = 0;
}

void SubTimer::update() {
    if (currentState != SubTimerState::RUNNING) return;
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastTick);
    if (duration.count() >= 1) {
        todayTotalTime++;
        cumulativeTotalTime++;
        m_historyMap[getCurrentDateString()] = todayTotalTime;
        lastTick = now;
    }
}

void SubTimer::setHistory(const std::map<std::string, long long>& history) {
    m_historyMap = history;
    cumulativeTotalTime = 0;
    todayTotalTime = 0;
    std::string todayStr = getCurrentDateString();
    for (const auto& [date, seconds] : m_historyMap) {
        cumulativeTotalTime += seconds;
        if (date == todayStr) todayTotalTime = seconds;
    }
}

void SubTimer::clickLogin() {
    currentState = SubTimerState::RUNNING;
    lastTick = std::chrono::steady_clock::now();
}
void SubTimer::clickRest() { currentState = SubTimerState::RESTING; }
void SubTimer::clickLogout() { currentState = SubTimerState::STOPPED; }

std::vector<SubRecord> SubTimer::getHistory() {
    std::vector<SubRecord> list;
    for (const auto& [date, seconds] : m_historyMap) {
        list.push_back({ date, seconds });
    }
    return list;
}

std::string SubTimer::getCurrentDateString() {
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);
    char buf[20];
    sprintf_s(buf, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    return std::string(buf);
}