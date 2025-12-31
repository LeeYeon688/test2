#pragma once

// 상태를 나타내는 열거형을 여기서만 '딱 한 번' 정의합니다.
// GameManager와 UIManager가 이 파일을 사이좋게 공유합니다.
enum class CurrentState {
    LOGGED_OUT, // 로그아웃 상태
    LOGGED_IN,  // 로그인 상태
    RESTING     // 휴식 상태
};