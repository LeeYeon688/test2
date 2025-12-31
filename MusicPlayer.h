#pragma once
#include <vector>
#include <string>
#include "ISoundDevice.h" // [변경] Soundbox.h 대신 인터페이스만 포함

// 재생 모드 (순차 재생 / 랜덤 재생)
enum class PlayMode { Sequential, Random };

// DJ의 현재 상태: 한 곡만 틀기 vs 리스트 틀기
enum class DJState { SingleLoop, Playlist };

class MusicPlayer {
public:
    // [수정] 구체적인 Soundbox 대신 추상적인 기계(ISoundDevice)를 받음
    MusicPlayer(ISoundDevice& soundDevice);

    // [기능 1] 단일 BGM 모드 (지금 원하시는 것)
    void playSingleBGM(const std::string& filePath);

    // [기능 2] 폴더 재생 모드 (나중에 쓸 것)
    void playFolder(const std::string& folderPath);

    // 매 프레임 호출: 음악이 끝났는지 감시하고 다음 곡 재생
    void update();

    // 강제로 다음 곡 재생
    void next();

    // 재생 모드 설정
    void setMode(PlayMode mode);

    // public에 추가
    void togglePause();

private:
    void playCurrentIndex(); // 내부적으로 현재 곡 재생 명령

private:
    // [수정] 리모컨(참조)의 타입 변경
    ISoundDevice& m_soundDevice;
    std::vector<std::string> m_playlist; // 파일 경로 목록
    int m_currentIndex;   // 현재 재생 중인 번호
    PlayMode m_currentMode;

    // 현재 DJ가 어떤 모드로 동작 중인지
    DJState m_state;
};
