#include "common.h"
#include "ui_core.h"
#include "app_scene.h"
#include "login_ui.h"

void print_title_segment();

void draw_login_screen()
{
    draw_box(0, 0, 119, 29); // 큰 박스
    print_title_segment();   // 로고

    draw_box(80, 14, 25, 3); // 아이디 박스
    draw_box(80, 17, 25, 3); // 비밀번호 박스

    goto_xy(106, 18); // 비밀번호 보기
    wprintf(L"[ ] view");

    goto_xy(88, 21); // 로그인 버튼
    wprintf(L"[ 로그인 ]");
    goto_xy(87, 23); // 회원가입 버튼
    wprintf(L"[ 회원가입 ]");
}

void print_title_segment()
{
    goto_xy(4, 2);
    wprintf(L"█████  █       ███   █   █  █   █  █████  ████\n");
    goto_xy(4, 3);
    wprintf(L"█   █  █      █   █  ██  █  ██  █  █      █   █\n");
    goto_xy(4, 4);
    wprintf(L"█   █  █      █   █  █ █ █  █ █ █  █      █   █\n");
    goto_xy(4, 5);
    wprintf(L"█████  █      █████  █  ██  █  ██  ████   ████\n");
    goto_xy(4, 6);
    wprintf(L"█      █      █   █  █   █  █   █  █      █ █\n");
    goto_xy(4, 7);
    wprintf(L"█      █      █   █  █   █  █   █  █      █  █\n");
    goto_xy(4, 8);
    wprintf(L"█      █████  █   █  █   █  █   █  █████  █   █\n");
}

void LoginUi_Draw( const wchar_t* id, const wchar_t* pw, LoginField currentField, int showPassword, int lastLoginFailed)
{
    // 1) 기본 로그인 레이아웃
    draw_login_screen();

    // 2) ID 내용 / 플레이스홀더 출력
    // ID 박스: draw_box(80,14,25,3) → 내부 텍스트 (82,15)
    goto_xy(82, 15);

    if (id[0] == L'\0' && currentField != FIELD_ID)
    {
        // 아직 아무것도 입력 안 했고, 포커스도 ID가 아닐 때 → "아이디"
        const wchar_t* placeholder_id = L"아이디";
        size_t len = wcslen(placeholder_id);

        wprintf(L"%ls", placeholder_id);
        while (len < 19)
        {
            wprintf(L" ");
            ++len;
        }
    }
    else
    {
        // 실제 입력 내용
        wprintf(L"%-21ls", id);
    }

    // 3) PW 내용 (마스킹 or 그대로) 준비
    wchar_t buf_pw[32];

    if (showPassword)
    {
        wcsncpy(buf_pw, pw, 31);
        buf_pw[31] = L'\0';
    }
    else
    {
        size_t len = wcslen(pw);
        size_t i;
        for (i = 0; i < len && i < 31; ++i)
        {
            buf_pw[i] = L'*';
        }
        buf_pw[i] = L'\0';
    }

    // PW 박스: draw_box(80,17,25,3) → 내부 텍스트 (82,18)
    goto_xy(82, 18);

    if (pw[0] == L'\0' && currentField != FIELD_PW)
    {
        // 아직 아무것도 입력 안 했고, 포커스도 PW가 아닐 때 → "비밀번호"
        const wchar_t* placeholder_pw = L"비밀번호";
        size_t len = wcslen(placeholder_pw);

        wprintf(L"%ls", placeholder_pw);
        while (len < 18)
        {
            wprintf(L" ");
            ++len;
        }
    }
    else
    {
        // 실제 PW(마스킹/실제값)
        wprintf(L"%-21ls", buf_pw);
    }

    // 4) view 체크박스
    goto_xy(106, 18);
    if (showPassword)
    {
        wprintf(L"[v] view");
    }
    else
    {
        wprintf(L"[ ] view");
    }

    // 5) 로그인 실패 메시지
    goto_xy(30, 13);
    if (lastLoginFailed)
    {
        wprintf(L"로그인에 실패했습니다. ID와 비밀번호를 확인하세요.");
    }
    else
    {
        wprintf(L"                                                  ");
    }
}