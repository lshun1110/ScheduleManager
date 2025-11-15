#include "common.h"
#include "login_logic.h"

// TODO: 나중에 user.txt 읽어서 실제로 검사하는 코드로 교체.
// 지금은 임시로 user_id="test", password="1234"만 성공 처리.
int Login_Auth(const wchar_t* user_id, const wchar_t* password, User* out_user)
{
    if (wcscmp(user_id, L"test") == 0 && wcscmp(password, L"1234") == 0)
    {
        wcsncpy(out_user->user_id, user_id, 31);
        out_user->user_id[31] = L'\0';

        wcsncpy(out_user->password, password, 31);
        out_user->password[31] = L'\0';

        wcsncpy(out_user->name, L"홍길동", 31);
        out_user->name[31] = L'\0';

        return 1;
    }

    return 0;
}