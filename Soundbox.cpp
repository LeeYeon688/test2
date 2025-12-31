#include "Soundbox.h"
#include <iostream>
#include <filesystem>

Soundbox::Soundbox() {
    m_bgm.setVolume(50.f); // 기본 볼륨
}

Soundbox::~Soundbox() {
    stop();
}

void Soundbox::play(const std::string& filePath) {
    // 파일이 없으면 에러 출력하고 끝
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "[Soundbox] 파일 없음: " << filePath << std::endl;
        return;
    }

    if (!m_bgm.openFromFile(filePath)) {
        std::cerr << "[Soundbox] 재생 실패: " << filePath << std::endl;
        return;
    }


    m_bgm.play();
    // std::cout << "[Soundbox] Playing: " << filePath << std::endl;
}

void Soundbox::stop() {
    if (m_bgm.getStatus() == sf::Music::Status::Playing) {
        m_bgm.stop();
    }
}

void Soundbox::setVolume(float volume) {
    m_bgm.setVolume(volume);
}

// 구현부 추가
void Soundbox::pause() {
    if (m_bgm.getStatus() == sf::Music::Status::Playing) {
        m_bgm.pause();
    }
}

void Soundbox::resume() {
    // 멈춰있(Paused)거나 정지(Stopped) 상태면 재생
    if (m_bgm.getStatus() != sf::Music::Status::Playing) {
        m_bgm.play();
    }
}

sf::Music::Status Soundbox::getStatus() const {
    return m_bgm.getStatus();
}

bool Soundbox::isStopped() const {
    return m_bgm.getStatus() == sf::Music::Status::Stopped;
}

// [추가 구현] SFML 3.0: setLooping / 2.x: setLoop
void Soundbox::setLooping(bool loop) {
    m_bgm.setLooping(loop);
}