#include "RecordUI.h"

RecordUI::RecordUI(const sf::Font& font)
    : m_isVisible(false)
    // [중요] 새로 추가한 라벨도 폰트로 초기화해야 합니다.
    //, m_txtMainLabel(font), m_txtMainTLabel(font)
    , m_txtMainDailyVal(font), m_txtMainTotalVal(font)
    //, m_txtSubLabel(font), m_txtSubTLabel(font)
    , m_txtSubDailyVal(font), m_txtSubTotalVal(font)
{
    sf::Color textColor(234, 237, 242); // 흰색 계열

    // [1] 메인 타이머 섹션
    // (1) 당일 라벨
    //m_txtMainLabel.setString("Today's Record");
    //m_txtMainLabel.setCharacterSize(50);
    //m_txtMainLabel.setFillColor(textColor);
    //m_txtMainLabel.setPosition({ 750.f, 375.f });

    // (2) 전체 라벨 [신규]
    //m_txtMainTLabel.setString("Total Record");
    //m_txtMainTLabel.setCharacterSize(50);
    //m_txtMainTLabel.setFillColor(textColor);
    //m_txtMainTLabel.setPosition({ 750.f, 510.f });

    // (3) 당일 값
    m_txtMainDailyVal.setCharacterSize(60);
    m_txtMainDailyVal.setFillColor(textColor);
    m_txtMainDailyVal.setPosition({ 800.f, 410.f });

    // (4) 전체 값
    m_txtMainTotalVal.setCharacterSize(60);
    m_txtMainTotalVal.setFillColor(textColor);
    m_txtMainTotalVal.setPosition({ 800.f, 540.f });

    // [2] 서브 타이머 섹션
    // (1) 당일 라벨
    //m_txtSubLabel.setString("Today's Sub Record");
    //m_txtSubLabel.setCharacterSize(50);
    //m_txtSubLabel.setFillColor(textColor);
    //m_txtSubLabel.setPosition({ 1530.f, 375.f });

    // (2) 전체 라벨 [신규]
    //m_txtSubTLabel.setString("Total Sub Record");
    //m_txtSubTLabel.setCharacterSize(50);
    //m_txtSubTLabel.setFillColor(textColor);
    //m_txtSubTLabel.setPosition({ 1530.f, 510.f });

    // (3) 당일 값
    m_txtSubDailyVal.setCharacterSize(60);
    m_txtSubDailyVal.setFillColor(textColor);
    m_txtSubDailyVal.setPosition({ 1600.f, 410.f });

    // (4) 전체 값
    m_txtSubTotalVal.setCharacterSize(60);
    m_txtSubTotalVal.setFillColor(textColor);
    m_txtSubTotalVal.setPosition({ 1600.f, 540.f });
}

void RecordUI::update(long long mainDaily, long long mainTotal, long long subDaily, long long subTotal) {
    if (!m_isVisible) return;

    // 사용자님이 작성하신 포맷("Today: " 등) 그대로 유지
    m_txtMainDailyVal.setString("Today: " + formatTime(mainDaily));
    m_txtMainTotalVal.setString("Total: " + formatTime(mainTotal));

    m_txtSubDailyVal.setString("Sub Today: " + formatTime(subDaily));
    m_txtSubTotalVal.setString("Sub Total: " + formatTime(subTotal));
}

void RecordUI::draw(sf::RenderWindow& window) {
    if (!m_isVisible) return;

    // 메인 그리기
    //window.draw(m_txtMainLabel);
    //window.draw(m_txtMainTLabel);     // [신규]
    window.draw(m_txtMainDailyVal);
    window.draw(m_txtMainTotalVal);

    // 서브 그리기
    //window.draw(m_txtSubLabel);
    //window.draw(m_txtSubTLabel);      // [신규]
    window.draw(m_txtSubDailyVal);
    window.draw(m_txtSubTotalVal);
}

std::string RecordUI::formatTime(long long s) {
    int h = (int)(s / 3600);
    int m = (int)((s % 3600) / 60);
    int sec = (int)(s % 60);
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << h << ":"
        << std::setfill('0') << std::setw(2) << m << ":"
        << std::setfill('0') << std::setw(2) << sec;
    return oss.str();
}