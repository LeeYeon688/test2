#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Enums.h"

// 전방 선언
class UIManager;
class TimerSystem;
class SubTimer;
class Soundbox;
class SystemManager;
class MusicPlayer;
class PlayerLv;
class PlayerData;
class RankBoard; // [필수] 전방 선언 추가

class GameManager {
public:
    GameManager();
    ~GameManager();

    void run();
    PlayerLv* getPlayerLv() const { return m_playerLv.get(); }

private:
    void processEvents();
    void update(sf::Time dt);
    void render();
    std::string formatTime(long long s);

private:
    sf::RenderWindow m_window;

    std::unique_ptr<UIManager> m_uiManager;
    std::unique_ptr<SystemManager> m_systemManager;
    std::unique_ptr<TimerSystem> m_mainTimer;
    std::unique_ptr<SubTimer> m_subTimer;
    std::unique_ptr<Soundbox> m_soundbox;
    std::unique_ptr<MusicPlayer> m_musicPlayer;
    std::unique_ptr<PlayerLv> m_playerLv;
    std::unique_ptr<PlayerData> m_playerData;
    // [필수] 이 줄이 없으면 GameManager.cpp에서 에러가 납니다.
    std::unique_ptr<RankBoard> m_rankBoard;

    CurrentState m_currentState;
};