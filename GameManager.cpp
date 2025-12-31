#include "GameManager.h"
#include "UIManager.h"
#include "TimerSystem.h"
#include "SubTimer.h"
#include "Soundbox.h"
#include "SystemManager.h"
#include "MusicPlayer.h"
#include "PlayerLv.h"
#include "PlayerData.h"
#include "RankBoard.h" // [필수]
#include "Structs.h" 

#include <cstdio>
#include <iostream>

GameManager::GameManager()
    : m_window(sf::VideoMode({ 1280, 720 }), "Melatonin Project")
{
    // 1. 의존성이 없는 객체들 먼저 생성
    m_uiManager = std::make_unique<UIManager>(m_window);
    m_mainTimer = std::make_unique<TimerSystem>();
    m_subTimer = std::make_unique<SubTimer>();
    m_soundbox = std::make_unique<Soundbox>();

    // [중요] SystemManager보다 먼저 만들어져야 함!
    m_playerLv = std::make_unique<PlayerLv>();
    m_rankBoard = std::make_unique<RankBoard>(); // [필수] RankBoard 생성
    m_playerData = std::make_unique<PlayerData>();
    m_musicPlayer = std::make_unique<MusicPlayer>(*m_soundbox);

    // 2. SystemManager 생성 (여기가 오류 발생 지점이었음)
    // 콤마(,)와 인자 순서를 정확하게 맞췄습니다.
    m_systemManager = std::make_unique<SystemManager>(
        m_window,          // 1. 윈도우
        *m_uiManager,      // 2. UI 컨트롤러 (UIManager가 IUIController를 상속받음)
        *m_musicPlayer,    // 3. 뮤직 플레이어
        *m_playerLv        // 4. 플레이어 상태
    );

    std::setlocale(LC_ALL, "Korean");
    m_window.setFramerateLimit(60);
    sf::View view(sf::FloatRect({ 0.f, 0.f }, { 2560.f, 1440.f }));
    m_window.setView(view);

    // [정상] 올바른 로드 (RankBoard 포함)
    m_playerData->load(*m_playerLv, *m_mainTimer, *m_subTimer, *m_rankBoard);

    m_uiManager->initialize();
    m_systemManager->initialize();

    m_currentState = CurrentState::LOGGED_OUT;
    m_uiManager->updateStatusImage(m_currentState);

    // 콜백 설정
    m_uiManager->setOnLoginCallback([this]() {
        m_currentState = CurrentState::LOGGED_IN;
        m_uiManager->updateStatusImage(m_currentState);
        m_mainTimer->clickLogin();
        m_subTimer->clickLogin();
        std::cout << "\n[Callback] 로그인 상태\n";
    });

    m_uiManager->setOnRestCallback([this]() {
        m_currentState = CurrentState::RESTING;
        m_uiManager->updateStatusImage(m_currentState);
        m_mainTimer->clickRest();
        m_subTimer->clickRest();
        std::cout << "\n[Callback] 휴식 상태\n";
    });

    m_uiManager->setOnLogoutCallback([this]() {
        m_currentState = CurrentState::LOGGED_OUT;
        m_uiManager->updateStatusImage(m_currentState);
        m_mainTimer->clickLogout();
        m_subTimer->clickLogout();
        std::cout << "\n[Callback] 로그아웃 상태\n";
    });

    std::string singleFile = R"(C:\Users\User\Desktop\C++\Audioclip\mainbgm\title.wav)";
    m_musicPlayer->playSingleBGM(singleFile);
}

GameManager::~GameManager() {
    // [수정] save 호출 시 RankBoard 전달
    if (m_playerData && m_playerLv && m_mainTimer && m_subTimer && m_rankBoard) {
        m_playerData->save(*m_playerLv, *m_mainTimer, *m_subTimer, *m_rankBoard);
    }
}

void GameManager::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        sf::Time dt = clock.restart();
        processEvents();
        update(dt);
        render();
    }
}

void GameManager::processEvents() {
    while (const auto event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }
        m_systemManager->handleInput(*event);
        m_uiManager->handleInput(*event);
    }
}

void GameManager::update(sf::Time dt) {
    m_musicPlayer->update();
    m_mainTimer->update();
    m_subTimer->update();

    m_uiManager->updateTimerText(
        formatTime(m_mainTimer->getTodayTime()),
        formatTime(m_subTimer->getTodayTime())
    );

    // =========================================================
    // [누락된 부분 복구] 기록 탭(RecordUI)에 데이터 전달
    // =========================================================
    if (m_mainTimer && m_subTimer) {
        long long mDaily = m_mainTimer->getTodayTime();
        long long mTotal = m_mainTimer->getTotalAccumulatedTime();
        long long sDaily = m_subTimer->getTodayTime();
        long long sTotal = m_subTimer->getTotalAccumulatedTime();

        // 이 함수가 호출되어야 비로소 숫자가 갱신됩니다!
        m_uiManager->updateRecordData(mDaily, mTotal, sDaily, sTotal);
    }

    // [RankBoard 관리] 주간 시간 누적 & 보상 체크
    if (m_mainTimer->isRunning()) {
        m_rankBoard->updateWeeklyTime(dt.asSeconds());
        m_rankBoard->processDailyReward(m_mainTimer->getTodayTime(), *m_playerLv);

        float currentHours = (float)m_mainTimer->getPureTime() / 3600.f;
        m_playerLv->updateWorkExperience(dt.asSeconds(), currentHours);
    }
    m_rankBoard->processWeeklyReset(*m_playerLv);

    if (m_mainTimer->isRunning()) {
        float currentHours = (float)m_mainTimer->getPureTime() / 3600.f;
        m_playerLv->updateWorkExperience(dt.asSeconds(), currentHours);
    }

    m_uiManager->update(dt);
    
    // 레벨 UI 갱신
    m_uiManager->updateLevelInfo(m_playerLv->getLevel(), m_playerLv->getCurrentLevelProgress());

    // [수정] 퀘스트 UI 갱신 (변경된 함수명 사용)
    m_uiManager->updateQuestData(
        m_playerLv->hasClaimed10HrBonus(),  // (구) SRank
        m_playerLv->hasClaimed12HrBonus()   // (구) SPlusRank
    );
}

void GameManager::render() {
    m_window.clear();
    m_uiManager->draw();
    m_window.display();
}

std::string GameManager::formatTime(long long s) {
    int h = (int)(s / 3600);
    int m = (int)((s % 3600) / 60);
    int sec = (int)(s % 60);
    char buf[20];
    sprintf_s(buf, "%02d:%02d:%02d", h, m, sec);
    return std::string(buf);
}