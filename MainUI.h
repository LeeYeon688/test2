#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include "Enums.h" 

class MainUI {
public:
    using Callback = std::function<void()>;

    // [수정] 생성자에서 폰트를 필수 재료로 받습니다.
    MainUI(sf::RenderWindow& window, const sf::Font& font);

    // [수정] 폰트는 이미 받았으니 여기서는 경로만 받습니다.
    void initialize(const std::string& rootPath);

    void handleInput(const sf::Event& event);
    void update(sf::Time dt);
    void draw();

    void updateTimerText(const std::string& mainTime, const std::string& subTime);
    void updateStatusImage(CurrentState state);
    // [신규] 레벨 및 경험치 업데이트 (LvUI 기능 통합)
    void updateLevelInfo(int level, float percentage);

    void fitBackgroundToWindow();

    void setOnLoginCallback(Callback callback) { m_onLoginClick = callback; }
    void setOnRestCallback(Callback callback) { m_onRestClick = callback; }
    void setOnLogoutCallback(Callback callback) { m_onLogoutClick = callback; }

    // [신규] 화면 표시 여부 설정
    void setVisible(bool visible) { m_isVisible = visible; }

private:
    sf::RenderWindow& m_window;

    // [신규] 화면 표시 여부 플래그 (기본값 true)
    bool m_isVisible = true;

    // 배경
    sf::Texture m_bgTexture;
    sf::Sprite m_bgSprite;

    // 버튼 및 UI
    sf::Texture m_texLogin, m_texRest, m_texLogout;
    sf::Sprite m_btnLogin, m_btnRest, m_btnLogout;

    sf::Texture m_texDashboard, m_texWork, m_texRun;
    sf::Sprite m_sprDashboard, m_sprWork, m_sprRun;

    // 타이머 텍스트
    sf::Text m_mainTimerText;
    sf::Text m_subTimerText;

    // 상태 아이콘
    sf::Texture m_texTypeLogin, m_texTypeRest, m_texTypeLogout;
    sf::Sprite m_sprType;
    // [LvUI 통합] 레벨 시스템 관련 변수
    sf::Text m_levelText;          // 레벨 텍스트
    sf::Texture m_texExpEmpty;     // 빈 구름
    sf::Texture m_texExpFull;      // 찬 구름
    sf::RectangleShape m_barExpEmpty;
    sf::RectangleShape m_barExpFull;

    sf::Vector2f m_expTargetSize;  // 구름 크기
    sf::Vector2f m_expPos;         // 구름 위치

    // 콜백
    Callback m_onLoginClick;
    Callback m_onRestClick;
    Callback m_onLogoutClick;
};