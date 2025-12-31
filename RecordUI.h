#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iomanip>
#include <sstream>

class RecordUI {
public:
    RecordUI(const sf::Font& font);

    void update(long long mainDaily, long long mainTotal, long long subDaily, long long subTotal);
    void draw(sf::RenderWindow& window);

    void setVisible(bool visible) { m_isVisible = visible; }
    bool isVisible() const { return m_isVisible; }

private:
    std::string formatTime(long long s);

private:
    bool m_isVisible;

    // [메인 타이머]
    //sf::Text m_txtMainLabel;    // "Today's Record"
    //sf::Text m_txtMainTLabel;   // "Total Record" [신규]
    sf::Text m_txtMainDailyVal; // 당일 값
    sf::Text m_txtMainTotalVal; // 누적 값

    // [서브 타이머]
    //sf::Text m_txtSubLabel;     // "Today's Sub Record"
    //sf::Text m_txtSubTLabel;    // "Total Sub Record" [신규]
    sf::Text m_txtSubDailyVal;  // 당일 값
    sf::Text m_txtSubTotalVal;  // 누적 값
};