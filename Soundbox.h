#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include "ISoundDevice.h" // [추가] 계약서 가져오기

// [수정] ISoundDevice를 상속받음
class Soundbox : public ISoundDevice {
public:
    Soundbox();
    ~Soundbox();

    // 인터페이스 구현 (override 키워드 추가)
    void play(const std::string& filePath) override;
    void stop() override;
    void pause() override;
    void resume() override;

    void setVolume(float volume) override;
    void setLooping(bool loop) override;

    bool isStopped() const override;
    
    // [신규] 인터페이스 요구사항 구현 (SFML 상태를 bool로 변환)
    bool isPlaying() const override {
        return m_bgm.getStatus() == sf::Music::Status::Playing;
    }

    // (참고) getStatus()는 인터페이스에 없으므로, 내부적으로만 쓰거나 제거해도 됩니다.
    // 여기서는 인터페이스 필수 함수인 isPlaying()으로 대체합니다.
    sf::Music::Status getStatus() const; 

private:
    sf::Music m_bgm;
};