#pragma once

// 시스템 관리자가 UI에게 요청할 수 있는 기능 목록 (인터페이스)
class IUIController {
public:
    virtual ~IUIController() {}

    // 해상도 변경 알림
    virtual void onResolutionChanged() = 0;

    // 탭 전환 기능
    virtual void switchToMainTab() = 0;
    virtual void switchToRecordTab() = 0;
    virtual void switchToQuestTab() = 0;
};