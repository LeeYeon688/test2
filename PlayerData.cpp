#include "PlayerData.h"
#include "PlayerLv.h"
#include "TimerSystem.h" 
#include "SubTimer.h"
#include "DateUtils.h" 
#include "RankBoard.h" 
#include <iostream>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

PlayerData::PlayerData() {}

// =============================================================
// [1] 통합 저장
// =============================================================
// [수정] 인자에 const RankBoard& rankBoard 추가됨
void PlayerData::save(const PlayerLv& playerLv, const TimerSystem& mainTimer, const SubTimer& subTimer, const RankBoard& rankBoard) {
    fs::path dir(SAVE_PATH_NEW);
    if (dir.has_parent_path() && !fs::exists(dir.parent_path())) {
        fs::create_directories(dir.parent_path());
    }

    std::string tempPath = SAVE_PATH_NEW + ".tmp";
    std::ofstream file(tempPath);

    if (file.is_open()) {
        file << "[XP]\n" << playerLv.getTotalXP() << "\n";

        // [DAILY_LEDGER] - 하나로 통합하여 저장 (중복 섹션 제거)
        file << "[DAILY_LEDGER]\n";
        file << DateUtils::getToday() << " "
            << playerLv.getTodayTimeXP() << " "
            << playerLv.getTodayTitleXP() << " "
            << (playerLv.hasClaimed10HrBonus() ? 1 : 0) << " "
            << (playerLv.hasClaimed12HrBonus() ? 1 : 0) << " "
            << (int)rankBoard.getDailyHighestPaidTier() << "\n";

        file << "[WEEKLY_DATA]\n";
        file << rankBoard.getWeeklyPlayTime() << " "
            << (rankBoard.getLastWeeklyResetDate().empty() ? "NONE" : rankBoard.getLastWeeklyResetDate()) << "\n";

        file << "[MAIN]\n";
        for (const auto& [date, seconds] : mainTimer.getHistoryMap()) {
            file << date << " " << secondsToTimeStr(seconds) << "\n";
        }

        file << "[SUB]\n";
        for (const auto& [date, seconds] : subTimer.getHistoryMap()) {
            file << date << " " << secondsToTimeStr(seconds) << "\n";
        }

        if (file.fail()) {
            file.close();
            fs::remove(tempPath);
            std::cerr << "[System] 오류: 데이터 저장 중 문제가 발생했습니다.\n";
            return;
        }
        file.close();

        try {
            if (fs::exists(SAVE_PATH_NEW)) fs::remove(SAVE_PATH_NEW);
            fs::rename(tempPath, SAVE_PATH_NEW);
            std::cout << "[System] 데이터 안전 저장 완료: " << SAVE_PATH_NEW << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "[System] 치명적 오류: 파일 교체 실패 (" << e.what() << ")\n";
        }
    }
}

// =============================================================
// [2] 통합 로드
// =============================================================
// [수정] 인자에 RankBoard& rankBoard 추가됨
void PlayerData::load(PlayerLv& playerLv, TimerSystem& mainTimer, SubTimer& subTimer, RankBoard& rankBoard) {
    if (fs::exists(SAVE_PATH_NEW)) {
        std::ifstream file(SAVE_PATH_NEW);
        if (file.is_open()) {
            std::string line, currentSection;
            long long loadedXP = 0;
            std::map<std::string, long long> mainHistory;
            std::map<std::string, long long> subHistory;

            while (std::getline(file, line)) {
                if (line.empty()) continue;
                if (line.front() == '[') { currentSection = line; continue; }

                if (currentSection == "[XP]") {
                    loadedXP = std::stoll(line);
                }
                else if (currentSection == "[DAILY_LEDGER]") {
                    std::stringstream ss(line);
                    std::string date;

                    // [중요] 변수를 여기서 미리 선언해야 에러가 안 납니다!
                    long long tXP = 0, titleXP = 0;
                    int bonus10 = 0, bonus12 = 0;
                    int tierVal = -1;

                    // 순서대로 읽기
                    if (ss >> date >> tXP >> titleXP) {
                        if (!(ss >> bonus10)) bonus10 = 0;
                        if (!(ss >> bonus12)) bonus12 = 0;
                        if (!(ss >> tierVal)) tierVal = -1;

                        if (date == DateUtils::getToday()) {
                            // 날짜가 같으면 복구
                            playerLv.setTodayTimeXP(tXP);
                            playerLv.setTodayTitleXP(titleXP);
                            playerLv.setClaimed10HrBonus(bonus10 == 1);
                            playerLv.setClaimed12HrBonus(bonus12 == 1);
                            rankBoard.setDailyHighestPaidTier((RankTier)tierVal);
                        }
                        else {
                            // 날짜 다르면 리셋
                            playerLv.setClaimed10HrBonus(false);
                            playerLv.setClaimed12HrBonus(false);
                            rankBoard.setDailyHighestPaidTier(RankTier::NONE);
                        }
                    }
                }
                else if (currentSection == "[WEEKLY_DATA]") {
                    std::stringstream ss(line);
                    long long wTime;
                    std::string wDate;
                    if (ss >> wTime >> wDate) {
                        rankBoard.setWeeklyPlayTime(wTime);
                        rankBoard.setLastWeeklyResetDate(wDate == "NONE" ? "" : wDate);
                    }
                }
                else if (currentSection == "[MAIN]") {
                    std::stringstream ss(line);
                    std::string date, valStr;
                    if (ss >> date >> valStr) {
                        mainHistory[date] = timeStrToSeconds(valStr);
                    }
                }
                else if (currentSection == "[SUB]") {
                    std::stringstream ss(line);
                    std::string date, valStr;
                    if (ss >> date >> valStr) {
                        subHistory[date] = timeStrToSeconds(valStr);
                    }
                }
            }
            file.close();

            playerLv.setTotalXP(loadedXP);
            mainTimer.setHistory(mainHistory);
            subTimer.setHistory(subHistory);
            std::cout << "[System] 통합 데이터 로드 성공.\n";
            return;
        }
    }

    std::cout << "[System] 통합 파일 없음. 기존 데이터 복구를 시작합니다...\n";
    loadLegacyData(playerLv, mainTimer, subTimer);
    // save 호출 시에도 rankBoard 전달
    save(playerLv, mainTimer, subTimer, rankBoard);
}

std::string PlayerData::secondsToTimeStr(long long s) {
    int h = (int)(s / 3600);
    int m = (int)((s % 3600) / 60);
    int sec = (int)(s % 60);
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << h << ":"
        << std::setfill('0') << std::setw(2) << m << ":"
        << std::setfill('0') << std::setw(2) << sec;
    return oss.str();
}

long long PlayerData::timeStrToSeconds(const std::string& timeStr) {
    if (timeStr.find(':') != std::string::npos) {
        int h, m, s;
        char c;
        std::stringstream ss(timeStr);
        ss >> h >> c >> m >> c >> s;
        return (h * 3600) + (m * 60) + s;
    }
    else {
        try { return std::stoll(timeStr); }
        catch (...) { return 0; }
    }
}

void PlayerData::loadLegacyData(PlayerLv& playerLv, TimerSystem& mainTimer, SubTimer& subTimer) {
    if (fs::exists(OLD_PATH_XP)) {
        std::ifstream file(OLD_PATH_XP);
        if (file.is_open()) {
            std::string line;
            std::getline(file, line);
            long long xp = 0;
            if (file >> xp) playerLv.setTotalXP(xp);
            file.close();
        }
    }
    if (fs::exists(OLD_PATH_MAIN)) {
        std::ifstream file(OLD_PATH_MAIN);
        std::map<std::string, long long> history;
        std::string date, timeStr;
        while (file >> date >> timeStr) {
            history[date] = timeStrToSeconds(timeStr);
        }
        mainTimer.setHistory(history);
    }
    if (fs::exists(OLD_PATH_SUB)) {
        std::ifstream file(OLD_PATH_SUB);
        std::map<std::string, long long> history;
        std::string date, timeStr;
        while (file >> date >> timeStr) {
            history[date] = timeStrToSeconds(timeStr);
        }
        subTimer.setHistory(history);
    }
}