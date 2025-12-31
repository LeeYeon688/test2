#include "DateUtils.h"
#include <ctime>
#include <iomanip> // std::setw, std::setfill
#include <sstream> // std::ostringstream

std::string DateUtils::getToday() {
    // 1. 현재 시스템 시간 가져오기
    time_t t = time(nullptr);
    struct tm tm;
    localtime_s(&tm, &t);

    // 2. 문자열 포맷팅 (YYYY-MM-DD) - C++ 스타일
    std::ostringstream oss;
    oss << (tm.tm_year + 1900) << "-"
        << std::setw(2) << std::setfill('0') << (tm.tm_mon + 1) << "-"
        << std::setw(2) << std::setfill('0') << tm.tm_mday;

    return oss.str();
}

bool DateUtils::isMonday() {
    time_t t = time(nullptr);
    struct tm tm;
    localtime_s(&tm, &t);

    // tm_wday: 0=일, 1=월, 2=화 ... 6=토
    return tm.tm_wday == 1;
}