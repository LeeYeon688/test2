#include "QuestUI.h"

QuestUI::QuestUI(const sf::Font& font)
    : m_isVisible(false)
    , m_txtTitle(font), m_txtQuestS(font), m_txtStatusS(font)
    , m_txtQuestSPlus(font), m_txtStatusSPlus(font)
{
    sf::Color colorTitle(255, 215, 0); // 골드
    sf::Color colorText(234, 237, 242); // 흰색 계열

    // 타이틀
    m_txtTitle.setString("QUESTS");
    m_txtTitle.setCharacterSize(60);
    m_txtTitle.setFillColor(colorTitle);
    m_txtTitle.setPosition({ 700.f, 350.f });

    // 1. S랭크 (10시간)
    m_txtQuestS.setString("- Over 10 Hours");
    m_txtQuestS.setCharacterSize(40);
    m_txtQuestS.setFillColor(colorText);
    m_txtQuestS.setPosition({ 700.f, 450.f });

    m_txtStatusS.setCharacterSize(40);
    m_txtStatusS.setPosition({ 1200.f, 450.f });

    // 2. S+랭크 (12시간)
    m_txtQuestSPlus.setString("- Over 12 Hours");
    m_txtQuestSPlus.setCharacterSize(40);
    m_txtQuestSPlus.setFillColor(colorText);
    m_txtQuestSPlus.setPosition({ 700.f, 550.f });

    m_txtStatusSPlus.setCharacterSize(40);
    m_txtStatusSPlus.setPosition({ 1200.f, 550.f });
}

void QuestUI::update(bool isClaimedS, bool isClaimedSPlus) {
    if (!m_isVisible) return;

    // 10시간 보상 상태
    if (isClaimedS) {
        m_txtStatusS.setString("[ COMPLETED ]");
        m_txtStatusS.setFillColor(sf::Color::Green);
    }
    else {
        m_txtStatusS.setString("[ INCOMPLETE ]");
        m_txtStatusS.setFillColor(sf::Color(150, 150, 150)); // 회색
    }

    // 12시간 보상 상태
    if (isClaimedSPlus) {
        m_txtStatusSPlus.setString("[ COMPLETED ]");
        m_txtStatusSPlus.setFillColor(sf::Color::Green);
    }
    else {
        m_txtStatusSPlus.setString("[ INCOMPLETE ]");
        m_txtStatusSPlus.setFillColor(sf::Color(150, 150, 150));
    }
}

void QuestUI::draw(sf::RenderWindow& window) {
    if (!m_isVisible) return;

    window.draw(m_txtTitle);
    window.draw(m_txtQuestS);
    window.draw(m_txtStatusS);
    window.draw(m_txtQuestSPlus);
    window.draw(m_txtStatusSPlus);
}