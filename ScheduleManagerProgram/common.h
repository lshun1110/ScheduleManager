#ifndef COMMON_H
#define COMMON_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <wchar.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

// 공통 상수
#define MAX_USER_NAME_LEN   32
#define MAX_TITLE_LEN       64
#define MAX_MEMO_LEN        128

// 카테고리 enum (파일로 안 빼기로 했으니까 enum 유지)
typedef enum {
    CATEGORY_DEFAULT = 0,
    CATEGORY_STUDY,
    CATEGORY_WORK,
    CATEGORY_EXERCISE,
    CATEGORY_ETC,
} Category;

// 유니코드 콘솔 초기화 함수 프로토타입
//void InitUnicodeConsole(void);

#endif // COMMON_H