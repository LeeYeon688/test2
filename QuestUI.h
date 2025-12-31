#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class QuestUI {
public:
    QuestUI(const sf::Font& font);

    // 보상 수령 여부를 받아 UI를 갱신
    void update(bool isClaimedS, bool isClaimedSPlus);
    void draw(sf::RenderWindow& window);

    void setVisible(bool visible) { m_isVisible = visible; }
    bool isVisible() const { return m_isVisible; }

private:
    bool m_isVisible;

    // UI 요소
    sf::Text m_txtTitle;

    // 퀘스트 1: 10시간
    sf::Text m_txtQuestS;
    sf::Text m_txtStatusS; // [완료] or [미완료]

    // 퀘스트 2: 12시간
    sf::Text m_txtQuestSPlus;
    sf::Text m_txtStatusSPlus;
};