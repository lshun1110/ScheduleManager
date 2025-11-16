#ifndef APP_SCENE_H
#define APP_SCENE_H

typedef enum
{
    SCENE_LOGIN = 0,
    SCENE_CALENDAR,
    SCENE_EXIT
} SceneState;

// 로그인 화면에서 포커스가 갈 수 있는 요소들
typedef enum
{
    FIELD_ID = 0,     // ID 입력 칸
    FIELD_PW,         // PW 입력 칸
    FIELD_LOGIN,      // [로그인] 버튼
    FIELD_VIEW,       // [ ] view 토글
    FIELD_COUNT       // 필드 개수 (순환용)
} LoginField;

#endif