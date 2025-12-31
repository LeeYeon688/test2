#include "SystemManager.h"
#include "MusicPlayer.h"
#include "PlayerStatus.h"

// [변경] 생성자 수정 (IUIController&)
SystemManager::SystemManager(sf::RenderWindow& window, IUIController& uiController, MusicPlayer& musicPlayer, const PlayerStatus& playerStatus)
    : m_window(window)
    , m_uiController(uiController) // [변경] 초기화
    , m_musicPlayer(musicPlayer)
    , m_playerStatus(playerStatus)
    , m_isConsoleVisible(false)
{
}

SystemManager::~SystemManager() {}

void SystemManager::initialize() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void SystemManager::setResolution(unsigned int width, unsigned int height) {
    m_window.create(sf::VideoMode({ width, height }), "Melatonin Project");
    m_window.setFramerateLimit(60);
    sf::View view(sf::FloatRect({ 0.f, 0.f }, { 2560.f, 1440.f }));
    m_window.setView(view);

    // [변경] UI 컨트롤러에게 알림
    m_uiController.onResolutionChanged();

    if (!m_isConsoleVisible) ShowWindow(GetConsoleWindow(), SW_HIDE);
    std::cout << "[System] 해상도 변경: " << width << "x" << height << std::endl;
}

void SystemManager::toggleConsole() {
    m_isConsoleVisible = !m_isConsoleVisible;
    HWND hConsole = GetConsoleWindow();
    if (m_isConsoleVisible) {
        ShowWindow(hConsole, SW_SHOWNOACTIVATE);
        std::cout << "[System] 콘솔 ON (` 키로 끄기)" << std::endl;
    }
    else {
        ShowWindow(hConsole, SW_HIDE);
    }
}

void SystemManager::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Grave) {
            toggleConsole();
        }

        // [탭 전환] 컨트롤러 함수 호출
        if (keyEvent->code == sf::Keyboard::Key::Numpad7) {
            m_uiController.switchToMainTab();
            std::cout << "[Tab] 메인 화면 전환\n";
        }
        else if (keyEvent->code == sf::Keyboard::Key::Numpad8) {
            m_uiController.switchToRecordTab();
            std::cout << "[Tab] 기록 화면 전환\n";
        }
        else if (keyEvent->code == sf::Keyboard::Key::Numpad9) {
            m_uiController.switchToQuestTab();
            std::cout << "[Tab] 퀘스트 화면 전환\n";
        }

        // 상태 확인 로직
        if (keyEvent->code == sf::Keyboard::Key::Numpad2) {
            if (!m_isConsoleVisible) toggleConsole();

            std::cout << "\n========== [PLAYER STATUS] ==========\n";
            std::cout << " 현재 레벨  : Lv. " << m_playerStatus.getLevel() << "\n";
            std::cout << " 보유 경험치: " << m_playerStatus.getTotalXP() << " XP\n";
            std::cout << " 진행률     : " << (m_playerStatus.getCurrentLevelProgress() * 100.0f) << "%\n";
            std::cout << "=====================================\n";
        }

        if (keyEvent->control && keyEvent->code == sf::Keyboard::Key::F1) {
            m_musicPlayer.togglePause();
        }
        if (keyEvent->control) {
            switch (keyEvent->code) {
            case sf::Keyboard::Key::F6: setResolution(640, 360); break;
            case sf::Keyboard::Key::F7: setResolution(1280, 720); break;
            case sf::Keyboard::Key::F8: setResolution(1440, 810); break;
            case sf::Keyboard::Key::F9: setResolution(1600, 900); break;
            case sf::Keyboard::Key::F10: setResolution(2560, 1440); break;
            }
        }
    }
}