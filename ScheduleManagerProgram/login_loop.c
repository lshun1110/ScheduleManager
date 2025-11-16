#include "common.h"
#include "app_scene.h"
#include "ui_core.h"
#include "login_ui.h"
#include "login_logic.h"
#include "login_loop.h"
#include "signup.h"
extern User g_current_user;

SceneState Login_Loop(void)
{
    wchar_t id[32] = L"";
    wchar_t pw[32] = L"";

    LoginField currentField = FIELD_ID;  // 처음에는 ID 칸
    int        showPassword = 0;        // 0: * 표시, 1: 실제 비밀번호 표시
    int        lastLoginFailed = 0;
    DWORD      failTick = 0;            // 로그인 실패 시각(ms)

    UiInputEvent ev;
    LoginView view = LOGIN_VIEW_LOGIN;  // 처음에는 로그인 화면

    // 클릭 판정용 영역들 (좌표는 LoginUi_Draw 기준)
    UiRect rect_id_box = (UiRect){ 80, 14, 25, 3 };  // ID 박스
    UiRect rect_pw_box = (UiRect){ 80, 17, 25, 3 };  // PW 박스
    UiRect rect_view_box = (UiRect){ 106, 18, 3, 1 };  // "[ ]" 부분
    UiRect rect_btn_login = (UiRect){ 88, 21, 11, 1 };  // "[ 로그인 ]"
    UiRect rect_btn_signup = (UiRect){ 87, 23, 13, 1 };  // "[ 회원가입 ]"

    while (1)
    {
        // 1) 실패 메시지 3초 유지 후 자동 제거
        if (lastLoginFailed)
        {
            DWORD now = GetTickCount();
            if (now - failTick >= 3000)   // 3000ms = 3초
            {
                lastLoginFailed = 0;
            }
        }

        // 2) 화면 그리기 (한 번에 하나만)
        if (view == LOGIN_VIEW_LOGIN)
        {
            int focus_is_id = (currentField == FIELD_ID);
            // LoginUi_Draw 안이 알아서 화면 지우기 + 그리기까지 처리한다고 가정
            LoginUi_Draw(id, pw, focus_is_id, showPassword, lastLoginFailed);
        }
        else // LOGIN_VIEW_SIGNUP
        {
            draw_signup_screen();
        }

        // 3) 커서 위치/표시 (로그인 화면에서만)
        if (view == LOGIN_VIEW_LOGIN)
        {
            if (currentField == FIELD_ID)
            {
                size_t len = wcslen(id);
                if (len > 21) len = 21;          // ID 텍스트 폭 21칸
                goto_xy(82 + (int)len, 15);      // ID 텍스트 시작 (82,15)
                set_cursor_visibility(1);
            }
            else if (currentField == FIELD_PW)
            {
                size_t len = wcslen(pw);
                if (len > 21) len = 21;          // PW 텍스트 폭 21칸
                goto_xy(82 + (int)len, 18);      // PW 텍스트 시작 (82,18)
                set_cursor_visibility(1);
            }
            else
            {
                set_cursor_visibility(0);
            }
        }
        else
        {
            set_cursor_visibility(0);
        }

        // 4) 입력 하나 대기
        Ui_WaitInput(&ev);

        // 5) 마우스 입력 처리
        if (ev.type == UI_INPUT_MOUSE_LEFT)
        {
            int mx = ev.pos.x;
            int my = ev.pos.y;

            if (view == LOGIN_VIEW_LOGIN)
            {
                // --- 로그인 화면에서의 클릭 처리 ---
                if (Ui_PointInRect(&rect_id_box, mx, my))
                {
                    currentField = FIELD_ID;      // ID 칸으로 포커스 이동
                }
                else if (Ui_PointInRect(&rect_pw_box, mx, my))
                {
                    currentField = FIELD_PW;      // PW 칸으로 포커스 이동
                }
                else if (Ui_PointInRect(&rect_btn_login, mx, my))
                {
                    // 로그인 시도
                    if (Login_Auth(id, pw, &g_current_user))
                    {
                        set_cursor_visibility(0);
                        // 로그인 성공 → 캘린더 Scene으로
                        return SCENE_CALENDAR;
                    }
                    else
                    {
                        lastLoginFailed = 1;
                        failTick = GetTickCount();   // 실패 시각 기록
                        currentField = FIELD_ID;
                    }
                }
                else if (Ui_PointInRect(&rect_btn_signup, mx, my))
                {
                    // 회원가입 화면으로 전환 (SceneState는 그대로 로그인 Scene 유지)
                    view = LOGIN_VIEW_SIGNUP;
                    currentField = FIELD_ID;  // 임시
                    continue;
                }
                else if (Ui_PointInRect(&rect_view_box, mx, my))
                {
                    currentField = FIELD_VIEW;
                    showPassword = !showPassword;
                }

                continue;
            }
            else
            {
                // --- 회원가입 화면에서의 클릭 처리 (추후 확장) ---
                // 예: 나중에 "뒤로가기" 버튼 만들면 여기서 view = LOGIN_VIEW_LOGIN; 처리
                // 지금은 아직 UI/로직 미구현 상태라 입력만 소비
                continue;
            }
        }

        // 6) 키보드 입력 처리
        if (ev.type == UI_INPUT_KEY)
        {
            wchar_t ch = ev.key;

            // ESC: 로그인 루프 자체 종료 → 프로그램 종료
            if (ch == 27)  // ESC
            {
                set_cursor_visibility(0);
                return SCENE_EXIT;
            }

            // 로그인 화면에서만 ID/PW 편집
            if (view == LOGIN_VIEW_LOGIN)
            {
                // TAB → 다음 필드
                if (ch == L'\t')
                {
                    currentField = (LoginField)(((int)currentField + 1) % FIELD_COUNT);
                }
                // BACKSPACE → 현재 텍스트 필드에서 한 글자 삭제
                else if (ch == L'\b')
                {
                    if (currentField == FIELD_ID)
                    {
                        size_t len = wcslen(id);
                        if (len > 0) id[len - 1] = L'\0';
                    }
                    else if (currentField == FIELD_PW)
                    {
                        size_t len = wcslen(pw);
                        if (len > 0) pw[len - 1] = L'\0';
                    }
                }
                // SPACE
                else if (ch == L' ')
                {
                    if (currentField == FIELD_VIEW)
                    {
                        showPassword = !showPassword;
                    }
                    else if (currentField == FIELD_ID || currentField == FIELD_PW)
                    {
                        wchar_t* buf = (currentField == FIELD_ID) ? id : pw;
                        size_t   len = wcslen(buf);

                        if (len < 31)
                        {
                            buf[len] = L' ';
                            buf[len + 1] = L'\0';
                        }
                    }
                }
                // 일반 인쇄 가능한 문자
                else if (iswprint(ch))
                {
                    if (currentField == FIELD_ID || currentField == FIELD_PW)
                    {
                        wchar_t* buf = (currentField == FIELD_ID) ? id : pw;
                        size_t   len = wcslen(buf);

                        if (len < 31)
                        {
                            buf[len] = ch;
                            buf[len + 1] = L'\0';
                        }
                    }
                }
            }
            else
            {
                // 회원가입 화면에서의 키보드 입력은 나중에 별도로 설계
                // (지금은 ESC로만 빠져나가게 해 둔 상태)
            }
        }
    }

    set_cursor_visibility(0);
    return SCENE_EXIT;
}