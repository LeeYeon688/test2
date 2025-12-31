#include "MusicPlayer.h"
#include <filesystem>
#include <iostream>
#include <random>

namespace fs = std::filesystem;

// [수정] 생성자 매개변수 이름 변경 (soundbox -> soundDevice)
MusicPlayer::MusicPlayer(ISoundDevice& soundDevice)
    : m_soundDevice(soundDevice) // 연결
    , m_currentIndex(0)
    , m_currentMode(PlayMode::Sequential)
    , m_state(DJState::SingleLoop)
{
}

void MusicPlayer::playSingleBGM(const std::string& filePath) {
    m_state = DJState::SingleLoop;
    m_playlist.clear();

    // m_soundbox -> m_soundDevice 로 이름 변경
    m_soundDevice.play(filePath);
    m_soundDevice.setLooping(true);

    std::cout << "[DJ] 단일 BGM 모드: " << filePath << std::endl;
}

void MusicPlayer::playFolder(const std::string& folderPath) {
    m_state = DJState::Playlist;
    m_playlist.clear();

    if (!fs::exists(folderPath)) return;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        std::string ext = entry.path().extension().string();
        if (ext == ".wav" || ext == ".mp3" || ext == ".ogg") {
            m_playlist.push_back(entry.path().string());
        }
    }

    if (!m_playlist.empty()) {
        m_soundDevice.setLooping(false);
        m_currentIndex = 0;
        playCurrentIndex();
        std::cout << "[DJ] 폴더 재생 모드 (" << m_playlist.size() << "곡)" << std::endl;
    }
}

void MusicPlayer::update() {
    if (m_playlist.empty()) return;

    if (m_soundDevice.isStopped()) {
        next();
    }
}

void MusicPlayer::next() {
    if (m_playlist.empty()) return;

    if (m_currentMode == PlayMode::Sequential) {
        m_currentIndex = (m_currentIndex + 1) % m_playlist.size();
    }
    else {
        m_currentIndex = rand() % m_playlist.size();
    }

    playCurrentIndex();
}

void MusicPlayer::setMode(PlayMode mode) {
    m_currentMode = mode;
    std::cout << "[DJ] 재생 모드 변경: " << (mode == PlayMode::Sequential ? "순차" : "랜덤") << std::endl;
}

void MusicPlayer::playCurrentIndex() {
    if (m_playlist.empty()) return;
    std::string songPath = m_playlist[m_currentIndex];
    m_soundDevice.play(songPath);
}

void MusicPlayer::togglePause() {
    // [수정] SFML 의존성(sf::Music::Status)을 없애고, 인터페이스 함수(isPlaying) 사용
    if (m_soundDevice.isPlaying()) {
        m_soundDevice.pause();
        std::cout << "[DJ] 음악 일시정지" << std::endl;
    }
    else {
        m_soundDevice.resume();
        std::cout << "[DJ] 음악 재개" << std::endl;
    }
}