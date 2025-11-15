#include "common.h"
#include "set_mode.h"
#include "ui_core.h"
#include "login_ui.h"
#include "app_scene.h"

SceneState current_scene = SCENE_LOGIN;
User g_current_user; // 현재 로그인한 사용자
int running = 1;

int main()
{
    set_console();

    SceneState scene = SCENE_LOGIN;
    int running = 1;
    
    while (running)
    {
        switch (scene)
        {
        case SCENE_LOGIN:
            scene = Login_Loop();   // <— Login 모듈로 위임
            break;

        case SCENE_CALENDAR:
            //scene = Calendar_Run();     // 캘린더 메인
            break;

        case SCENE_EXIT:
            running = 0;
            break;
        }
    }

    return 0;
}