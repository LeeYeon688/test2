#include "TimerSystem.h"
#include <iostream>
#include <ctime>

TimerSystem::TimerSystem() {
    currentState = TimerState::STOPPED;
    mainSessionTime = 0;
    todayTotalTime = 0;
    cumulativeTotalTime = 0;
    // 생성자에서 load() 삭제 (PlayerData가 해줌)
}

void TimerSystem::update() {
    if (currentState != TimerState::RUNNING) return;

    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastTick);

    if (duration.count() >= 1) {
        if (checkPhotoshopActive()) {
            mainSessionTime++;
            todayTotalTime++;
            cumulativeTotalTime++;

            // 실시간 기록 업데이트
            m_historyMap[getCurrentDateString()] = todayTotalTime;
        }
        lastTick = now;
    }
}

// [중요] 외부에서 데이터 주입 (복구용)
void TimerSystem::setHistory(const std::map<std::string, long long>& history) {
    m_historyMap = history;

    // 오늘 날짜 데이터 찾아서 복구
    cumulativeTotalTime = 0;
    todayTotalTime = 0;
    std::string todayStr = getCurrentDateString();

    for (const auto& [date, seconds] : m_historyMap) {
        cumulativeTotalTime += seconds;
        if (date == todayStr) todayTotalTime = seconds;
    }
}

void TimerSystem::clickLogin() {
    currentState = TimerState::RUNNING;
    lastTick = std::chrono::steady_clock::now();
}
void TimerSystem::clickRest() { currentState = TimerState::RESTING; }
void TimerSystem::clickLogout() {
    currentState = TimerState::STOPPED;
    mainSessionTime = 0;
    // save() 호출 삭제
}

std::vector<DailyRecord> TimerSystem::getHistory() {
    std::vector<DailyRecord> list;
    for (const auto& [date, seconds] : m_historyMap) {
        list.push_back({ date, seconds });
    }
    return list;
}

// ... (checkPhotoshopActive, getCurrentDateString 등 나머지 함수는 기존 그대로 유지) ...
// 아래 함수들은 기존 코드에서 복사해서 쓰시면 됩니다.
bool TimerSystem::checkPhotoshopActive() {
    HWND hwnd = GetForegroundWindow();
    if (hwnd == NULL) return false;
    DWORD processID;
    GetWindowThreadProcessId(hwnd, &processID);
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess == NULL) return false;
    char processName[MAX_PATH] = "<unknown>";
    if (GetModuleBaseNameA(hProcess, NULL, processName, sizeof(processName) / sizeof(char))) {}
    CloseHandle(hProcess);
    std::string procName(processName);
    if (procName.find("Photoshop.exe") != std::string::npos) return true;
    return false;
}

std::string TimerSystem::getCurrentDateString() {
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);
    char buf[20];
    sprintf_s(buf, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    return std::string(buf);
}

int TimerSystem::getCurrentDay() const {
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);
    return tm.tm_mday;
}