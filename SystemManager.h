#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <iostream>

// [중요] 인터페이스 헤더 포함
#include "IUIController.h" 

class MusicPlayer;
class PlayerStatus;

class SystemManager {
public:
    // [중요] IResolutionListener 대신 IUIController&를 받습니다.
    SystemManager(sf::RenderWindow& window, IUIController& uiController, MusicPlayer& musicPlayer, const PlayerStatus& playerStatus);
    ~SystemManager();

    void initialize();
    void handleInput(const sf::Event& event);
    void setResolution(unsigned int width, unsigned int height);
    void toggleConsole();

private:
    sf::RenderWindow& m_window;

    // 인터페이스 참조
    IUIController& m_uiController;

    MusicPlayer& m_musicPlayer;
    const PlayerStatus& m_playerStatus;

    bool m_isConsoleVisible;
};