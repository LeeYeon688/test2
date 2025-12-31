#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <map>
#include <vector>

class PlayerLv;
class TimerSystem;
class SubTimer;
class RankBoard; // [추가]

class PlayerData {
public:
    PlayerData();

    // [수정] RankBoard& 인자 추가 확인
    void save(const PlayerLv& playerLv, const TimerSystem& mainTimer, const SubTimer& subTimer, const RankBoard& rankBoard);
    void load(PlayerLv& playerLv, TimerSystem& mainTimer, SubTimer& subTimer, RankBoard& rankBoard);

private:
    // [기능 추가] 문자열(00:00:00) <-> 초(Seconds) 변환 함수들
    long long timeStrToSeconds(const std::string& timeStr);
    std::string secondsToTimeStr(long long seconds);

    void loadLegacyData(PlayerLv& playerLv, TimerSystem& mainTimer, SubTimer& subTimer);

private:
    const std::string SAVE_PATH_NEW = R"(C:\Users\User\Desktop\C++\savedata\GameData.txt)";

    const std::string OLD_PATH_XP = R"(C:\Users\User\Desktop\C++\savedata\Playerdata\Playerdata.txt)";
    const std::string OLD_PATH_MAIN = R"(C:\Users\User\Desktop\C++\savedata\Main\records.txt)";
    const std::string OLD_PATH_SUB = R"(C:\Users\User\Desktop\C++\savedata\Sub\records.txt)";
};