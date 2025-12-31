#include "UIManager.h"
#include "MainUI.h" 
#include "RecordUI.h"
#include "QuestUI.h"
#include <iostream>

UIManager::UIManager(sf::RenderWindow& window)
    : m_window(window)
    , m_currentTab(ActiveTab::MAIN) // 시작은 메인 탭
{
}

UIManager::~UIManager() {}

void UIManager::initialize() {
    std::string fontPath = R"(C:\Users\User\Desktop\C++\font\Typoghost.ttf)";
    if (!m_font.openFromFile(fontPath)) {
        if (!m_font.openFromFile("C:\\Windows\\Fonts\\arial.ttf"))
            std::cerr << "[치명적] 폰트 로드 실패\n";
    }

    // 각 UI 생성
    m_mainUI = std::make_unique<MainUI>(m_window, m_font);
    std::string rootPath = R"(C:\Users\User\Desktop\C++\)";
    m_mainUI->initialize(rootPath);

    m_recordUI = std::make_unique<RecordUI>(m_font);
    m_questUI = std::make_unique<QuestUI>(m_font);

    // 초기 상태 설정
    switchToMainTab();
}

void UIManager::update(sf::Time dt) {
    // 메인 탭일 때만 메인 UI 애니메이션 업데이트
    if (m_currentTab == ActiveTab::MAIN && m_mainUI) {
        m_mainUI->update(dt);
    }
}

// -------------------------------------------------------------
// 데이터 업데이트 함수들
// -------------------------------------------------------------
void UIManager::updateLevelInfo(int level, float percentage) {
    if (m_mainUI) m_mainUI->updateLevelInfo(level, percentage);
}

void UIManager::updateRecordData(long long mDaily, long long mTotal, long long sDaily, long long sTotal) {
    if (m_recordUI) m_recordUI->update(mDaily, mTotal, sDaily, sTotal);
}

void UIManager::updateQuestData(bool bClaimedS, bool bClaimedSPlus) {
    if (m_questUI) m_questUI->update(bClaimedS, bClaimedSPlus);
}

// -------------------------------------------------------------
// 입력 및 그리기
// -------------------------------------------------------------
void UIManager::handleInput(const sf::Event& event) {
    // 탭에 상관없이 메인 UI(버튼, 단축키)는 항상 입력을 받음
    if (m_mainUI) {
        m_mainUI->handleInput(event);
    }
}

void UIManager::draw() {
    // 1. 공통 배경과 버튼 그리기 (MainUI가 담당)
    if (m_mainUI) m_mainUI->draw();

    // 2. 탭별 콘텐츠 그리기
    switch (m_currentTab) {
    case ActiveTab::MAIN:
        // MainUI::draw 내부에서 이미 그려짐 (visible 체크됨)
        break;
    case ActiveTab::RECORD:
        if (m_recordUI) m_recordUI->draw(m_window);
        break;
    case ActiveTab::QUEST:
        if (m_questUI) m_questUI->draw(m_window);
        break;
    }
}

// -------------------------------------------------------------
// 탭 전환 로직
// -------------------------------------------------------------
void UIManager::switchToMainTab() {
    m_currentTab = ActiveTab::MAIN;
    // MainUI가 버튼 + 대시보드 모두 표시
    if (m_mainUI) m_mainUI->setVisible(true);
    if (m_recordUI) m_recordUI->setVisible(false);
    if (m_questUI) m_questUI->setVisible(false);
}

void UIManager::switchToRecordTab() {
    m_currentTab = ActiveTab::RECORD;
    // MainUI는 버튼만 표시(false면 draw에서 분기처리됨)
    if (m_mainUI) m_mainUI->setVisible(false);
    if (m_recordUI) m_recordUI->setVisible(true);
    if (m_questUI) m_questUI->setVisible(false);
}

void UIManager::switchToQuestTab() {
    m_currentTab = ActiveTab::QUEST;
    if (m_mainUI) m_mainUI->setVisible(false);
    if (m_recordUI) m_recordUI->setVisible(false);
    if (m_questUI) m_questUI->setVisible(true);
}

// -------------------------------------------------------------
// 위임 함수들
// -------------------------------------------------------------
void UIManager::updateTimerText(const std::string& mainTime, const std::string& subTime) {
    if (m_mainUI) m_mainUI->updateTimerText(mainTime, subTime);
}

void UIManager::updateStatusImage(CurrentState state) {
    if (m_mainUI) m_mainUI->updateStatusImage(state);
}

void UIManager::fitBackgroundToWindow() {
    if (m_mainUI) m_mainUI->fitBackgroundToWindow();
}

void UIManager::setOnLoginCallback(Callback callback) {
    if (m_mainUI) m_mainUI->setOnLoginCallback(callback);
}

void UIManager::setOnRestCallback(Callback callback) {
    if (m_mainUI) m_mainUI->setOnRestCallback(callback);
}

void UIManager::setOnLogoutCallback(Callback callback) {
    if (m_mainUI) m_mainUI->setOnLogoutCallback(callback);
}