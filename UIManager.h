#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <memory>
#include "Enums.h" 
#include "IUIController.h" // [변경] 인터페이스 포함

class MainUI;
class RecordUI;
class QuestUI; // [신규] 전방 선언

// [중요] 탭 상태 정의 (클래스 밖이나 위쪽에 있어야 인식됨)
enum class ActiveTab { MAIN, RECORD, QUEST };

// [변경] IResolutionListener 대신 IUIController를 상속받습니다.
// (IUIController 안에 onResolutionChanged가 있으므로 기능은 유지됩니다)
class UIManager : public IUIController {
public:
    using Callback = std::function<void()>;

    UIManager(sf::RenderWindow& window);
    ~UIManager();

    void initialize();
    void update(sf::Time dt);
    void handleInput(const sf::Event& event);
    void draw();

    void updateTimerText(const std::string& mainTime, const std::string& subTime);
    void updateRecordData(long long mDaily, long long mTotal, long long sDaily, long long sTotal);
    // [중요] 이 함수가 선언되어 있어야 GameManager에서 에러가 안 납니다!
    void updateLevelInfo(int level, float percentage);

    // [신규] 퀘스트 정보 업데이트
    void updateQuestData(bool bClaimedS, bool bClaimedSPlus);

    void updateStatusImage(CurrentState state);
    void fitBackgroundToWindow();

    // [IUIController 구현] Override 키워드 명시
    void onResolutionChanged() override {
        fitBackgroundToWindow();
    }
    void switchToMainTab() override;
    void switchToRecordTab() override;
    void switchToQuestTab() override; // [신규]

    void setOnLoginCallback(Callback callback);
    void setOnRestCallback(Callback callback);
    void setOnLogoutCallback(Callback callback);
    const sf::Font& getFont() const { return m_font; }

private:
    sf::RenderWindow& m_window;
    sf::Font m_font;

    std::unique_ptr<MainUI> m_mainUI;
    std::unique_ptr<RecordUI> m_recordUI;
    std::unique_ptr<QuestUI> m_questUI; // [신규]
    // [중요] 현재 탭 상태 저장 변수 (기존 bool m_showMainUI 대체)
    ActiveTab m_currentTab;
};