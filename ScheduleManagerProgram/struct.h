#pragma once

typedef struct {
    char userID[20];      // 로그인할 때 쓰는 ID이자, 다른 파일에서 참조하는 키
    char password[20];    // 비밀번호
    wchar_t userName[30];    // 사용자 이름(표시용)
} User;