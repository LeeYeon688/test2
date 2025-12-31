#include "MainUI.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

// [수정] 생성자에서 폰트를 받아서 텍스트(m_mainTimerText)에 바로 넣어줍니다.
MainUI::MainUI(sf::RenderWindow& window, const sf::Font& font)
    : m_window(window)
    , m_bgSprite(m_bgTexture)
    , m_btnLogin(m_texLogin), m_btnRest(m_texRest), m_btnLogout(m_texLogout)
    , m_sprDashboard(m_texDashboard), m_sprWork(m_texWork), m_sprRun(m_texRun)
    , m_sprType(m_texTypeLogout)
    // ★★★ 여기가 핵심입니다! 폰트를 넣어서 초기화합니다. ★★★
    , m_mainTimerText(font)
    , m_subTimerText(font)
    // [LvUI 통합] 레벨 텍스트 초기화
    , m_levelText(font)
{
}

// [수정] 폰트 인자 제거 (생성자에서 이미 해결함)
void MainUI::initialize(const std::string& rootPath) {

    // 2. 리소스 로딩 람다
    auto loadTex = [&](sf::Texture& tex, sf::Sprite& spr, std::string subPath) {
        std::string fullPath = rootPath + subPath;
        if (!fs::exists(fullPath)) return;
        if (tex.loadFromFile(fullPath)) {
            tex.setSmooth(true);
            spr.setTexture(tex, true);
        }
        };

    // 3. 이미지 로드
    if (m_bgTexture.loadFromFile(rootPath + R"(png\Background.png)")) {
        m_bgTexture.setSmooth(true);
        m_bgSprite.setTexture(m_bgTexture, true);
        fitBackgroundToWindow();
    }

    loadTex(m_texLogin, m_btnLogin, R"(sprite\login.PNG)");
    loadTex(m_texRest, m_btnRest, R"(sprite\rest.png)");
    loadTex(m_texLogout, m_btnLogout, R"(sprite\logout.png)");
    loadTex(m_texDashboard, m_sprDashboard, R"(sprite\tdashboard.png)");
    loadTex(m_texWork, m_sprWork, R"(sprite\work.png)");
    loadTex(m_texRun, m_sprRun, R"(sprite\run.png)");

    // 상태 아이콘 로드
    if (m_texTypeLogin.loadFromFile(rootPath + R"(sprite\typelogin.png)")) m_texTypeLogin.setSmooth(true);
    if (m_texTypeRest.loadFromFile(rootPath + R"(sprite\typerest.png)")) m_texTypeRest.setSmooth(true);
    if (m_texTypeLogout.loadFromFile(rootPath + R"(sprite\typelogout.png)")) m_texTypeLogout.setSmooth(true);

    // 4. 좌표 설정
    m_btnLogin.setPosition({ 769.f, 243.f });
    m_btnRest.setPosition({ 1171.f, 251.f });
    m_btnLogout.setPosition({ 1572.f, 250.f });
    m_sprDashboard.setPosition({ 53.f, 169.f });
    m_sprWork.setPosition({ 1453.f, 375.f });
    m_sprRun.setPosition({ 1284.f, 31.f });

    // 5. 텍스트 설정 (폰트는 이미 생성자에서 설정됨)
    // setFont 호출 불필요 (하지만 안전상 놔둬도 됨, 여기선 제거)
    m_mainTimerText.setCharacterSize(90);
    m_mainTimerText.setFillColor(sf::Color(218, 148, 255));
    m_mainTimerText.setPosition({ 1562.f, 450.f });
    m_mainTimerText.setString("00:00:00");

    m_subTimerText.setCharacterSize(90);
    m_subTimerText.setFillColor(sf::Color(218, 148, 255));
    m_subTimerText.setPosition({ 1562.f, 570.f });
    m_subTimerText.setString("00:00:00");
    // --------------------------------------------------------
        // [2] LvUI 통합 로직 (경험치 구름 및 텍스트)
        // --------------------------------------------------------
    m_levelText.setCharacterSize(60);
    m_levelText.setFillColor(sf::Color(234, 237, 242));
    m_levelText.setPosition({ 750.f, 380.f });
    m_levelText.setString("Lv. 1");

    std::string spritePath = rootPath + R"(sprite\)";

    if (m_texExpEmpty.loadFromFile(spritePath + "emptyexp.png")) m_texExpEmpty.setSmooth(true);
    else std::cerr << "[MainUI] emptyexp.png 로드 실패\n";

    if (m_texExpFull.loadFromFile(spritePath + "fullexp.png")) m_texExpFull.setSmooth(true);
    else std::cerr << "[MainUI] fullexp.png 로드 실패\n";

    // 구름 위치 및 크기 설정
    m_expTargetSize = { 268.f, 150.f };
    m_expPos = { 910.f, 340.f };

    // 배경 구름
    m_barExpEmpty.setSize(m_expTargetSize);
    m_barExpEmpty.setTexture(&m_texExpEmpty);
    m_barExpEmpty.setPosition(m_expPos);

    // 차오르는 구름 (초기값 0)
    m_barExpFull.setSize(m_expTargetSize);
    m_barExpFull.setTexture(&m_texExpFull);
    m_barExpFull.setPosition(m_expPos);

    sf::Vector2u texSize = m_texExpFull.getSize();
    m_barExpFull.setTextureRect(sf::IntRect({ 0, (int)texSize.y }, { (int)texSize.x, 0 }));
    m_barExpFull.setSize({ m_expTargetSize.x, 0.f });
}
// [신규] 레벨 및 경험치바 업데이트 함수
void MainUI::updateLevelInfo(int level, float percentage) {
    // 1. 텍스트 갱신
    m_levelText.setString("LV. " + std::to_string(level));

    // 2. 구름 게이지 차오름 효과 (LvUI 로직 이식)
    sf::Vector2u texSize = m_texExpFull.getSize();
    int visibleTexHeight = (int)(texSize.y * percentage);

    // 텍스처 자르기 (아래에서 위로)
    sf::IntRect rect(
        { 0, (int)texSize.y - visibleTexHeight },
        { (int)texSize.x, visibleTexHeight }
    );
    m_barExpFull.setTextureRect(rect);

    // 화면 표시 크기 및 위치 보정
    float currentScreenHeight = m_expTargetSize.y * percentage;
    m_barExpFull.setSize({ m_expTargetSize.x, currentScreenHeight });
    m_barExpFull.setPosition({ m_expPos.x, m_expPos.y + (m_expTargetSize.y - currentScreenHeight) });
}

void MainUI::updateStatusImage(CurrentState state) {
    switch (state) {
    case CurrentState::LOGGED_IN:
        m_sprType.setTexture(m_texTypeLogin, true);
        m_sprType.setPosition({ 556.f, 73.f });
        break;
    case CurrentState::RESTING:
        m_sprType.setTexture(m_texTypeRest, true);
        m_sprType.setPosition({ 556.f, 83.f });
        break;
    case CurrentState::LOGGED_OUT:
        m_sprType.setTexture(m_texTypeLogout, true);
        m_sprType.setPosition({ 556.f, 76.f });
        break;
    }
}

void MainUI::updateTimerText(const std::string& mainTime, const std::string& subTime) {
    m_mainTimerText.setString(mainTime);
    m_subTimerText.setString(subTime);
}

void MainUI::update(sf::Time dt) {}

void MainUI::handleInput(const sf::Event& event) {
    // 화면이 안 보이면 클릭도 막음
    //if (!m_isVisible) return; /

    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = m_window.mapPixelToCoords(mouseEvent->position);

            if (m_btnLogin.getGlobalBounds().contains(mousePos) && m_onLoginClick) m_onLoginClick();
            else if (m_btnRest.getGlobalBounds().contains(mousePos) && m_onRestClick) m_onRestClick();
            else if (m_btnLogout.getGlobalBounds().contains(mousePos) && m_onLogoutClick) m_onLogoutClick();
        }
    }
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::F1 && m_onLoginClick) m_onLoginClick();
        else if (keyEvent->code == sf::Keyboard::Key::F2 && m_onRestClick) m_onRestClick();
        else if (keyEvent->code == sf::Keyboard::Key::F3 && m_onLogoutClick) m_onLogoutClick();
    }
}

void MainUI::draw() {
    // --------------------------------------------------------
    // 1. [항상 그리기] 배경 (가장 먼저 그려서 바닥에 깔기)
    // --------------------------------------------------------
    m_window.draw(m_bgSprite);

    // --------------------------------------------------------
    // 2. [항상 그리기] 공통 UI (기록 탭에서도 보여야 하는 것들)
    // --------------------------------------------------------
    // 컨트롤 버튼들
    m_window.draw(m_btnLogin);
    m_window.draw(m_btnRest);
    m_window.draw(m_btnLogout);
    m_window.draw(m_sprDashboard);

    // 상태 표시 아이콘 (로그인/휴식 등)
    m_window.draw(m_sprType);
    m_window.draw(m_sprRun); // (아까 위에서 지우고 여기로 내렸습니다)
    // --------------------------------------------------------
    // [분기점] 여기서부터는 '메인 탭'이 켜져 있을 때만 그립니다.
    // --------------------------------------------------------
    if (!m_isVisible) return;

    // 메인 대시보드 전용 이미지들
    m_window.draw(m_sprWork);

    // 타이머 텍스트
    m_window.draw(m_mainTimerText);
    m_window.draw(m_subTimerText);

    // 레벨 및 경험치 게이지
    m_window.draw(m_levelText);
    m_window.draw(m_barExpEmpty); // 빈 구름
    m_window.draw(m_barExpFull);  // 차오르는 구름
}

void MainUI::fitBackgroundToWindow() {
    sf::Vector2u texSize = m_bgTexture.getSize();
    sf::Vector2f viewSize = m_window.getView().getSize();
    if (texSize.x != 0 && texSize.y != 0) {
        m_bgSprite.setScale({ viewSize.x / texSize.x, viewSize.y / texSize.y });
        m_bgSprite.setPosition({ 0.f, 0.f });
    }
}