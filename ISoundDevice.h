#pragma once
#include <string>

// 모든 오디오 장치(스피커)가 지켜야 할 약속
class ISoundDevice {
public:
    virtual ~ISoundDevice() {}

    // 재생, 정지, 일시정지, 재개
    virtual void play(const std::string& filePath) = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;

    // 설정
    virtual void setVolume(float volume) = 0;
    virtual void setLooping(bool loop) = 0;

    // 상태 확인 (SFML 의존성을 없애기 위해 bool로 추상화)
    virtual bool isStopped() const = 0;
    virtual bool isPlaying() const = 0;
};