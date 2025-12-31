#pragma once

// "플레이어의 상태 정보를 조회할 수 있는 인터페이스"
class PlayerStatus {
public:
    virtual ~PlayerStatus() {}

    // 레벨, 경험치, 진행률을 확인하는 함수들 (구현은 PlayerLv가 함)
    virtual int getLevel() const = 0;
    virtual long long getTotalXP() const = 0;
    virtual float getCurrentLevelProgress() const = 0;

    // 칭호 정보도 필요하다면 여기서 약속
    // virtual const TitleBonus& getCurrentTitle() const = 0;
};