#include "common.h"
#include "login_logic.h"

// ----------------------------------------------
// [1] user.txt 열기 전용 함수 (읽기 모드)
// ----------------------------------------------
static FILE* OpenUserFileRead(void)
{
    // 프로젝트 실행 파일과 같은 폴더에 있는 user.txt를 연다고 가정
    // UTF-16 텍스트 모드
    return _wfopen(L"user.txt", L"rt, ccs=UNICODE");
}

// ----------------------------------------------
// [2] 파일에서 User 한 명 정보를 읽어오는 함수
//   - 성공: 1 리턴, out_user에 데이터 채움
//   - 실패(EOF 또는 형식 이상): 0 리턴
// ----------------------------------------------
static int ReadUserRecord(FILE* fp, User* out_user)
{
    wchar_t id[32];
    wchar_t pw[32];
    wchar_t name[32];
    int count;

    if (fp == NULL || out_user == NULL)
        return 0;

    // 한 줄에서 user_id, password, name 읽기
    count = fwscanf(fp, L"%31ls %31ls %31ls", id, pw, name);
    if (count != 3)
    {
        // EOF거나 형식이 잘못된 경우
        return 0;
    }

    // 읽어 온 값을 User 구조체에 복사
    wcsncpy(out_user->user_id, id, 31);
    out_user->user_id[31] = L'\0';

    wcsncpy(out_user->password, pw, 31);
    out_user->password[31] = L'\0';

    wcsncpy(out_user->name, name, 31);
    out_user->name[31] = L'\0';

    return 1;
}
// ----------------------------------------------
// [3] 로그인 인증 함수 (외부에 제공되는 API)
//   - user_id, password를 받아서
//     user.txt를 순회하며 일치하는 계정이 있는지 검사.
//   - 찾으면 out_user에 정보 채우고 1 리턴
//   - 없으면 0 리턴
// ----------------------------------------------
int Login_Auth(const wchar_t* user_id,
    const wchar_t* password,
    User* out_user)
{
    FILE* fp;
    User tmp_user;

    fp = OpenUserFileRead();
    if (fp == NULL)
    {
        // 파일 없음/열기 실패 → 로그인 실패 처리
        return 0;
    }

    // 파일의 모든 User 레코드를 순서대로 검사
    while (ReadUserRecord(fp, &tmp_user))
    {
        if (wcscmp(user_id, tmp_user.user_id) == 0 &&
            wcscmp(password, tmp_user.password) == 0)
        {
            // 로그인 성공
            if (out_user != NULL)
            {
                // 구조체 통째로 복사
                *out_user = tmp_user;
            }

            fclose(fp);
            return 1;
        }
    }

    // 여기까지 왔다 = 일치하는 계정 없음
    fclose(fp);
    return 0;
}