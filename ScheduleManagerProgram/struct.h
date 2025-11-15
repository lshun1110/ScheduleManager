#ifndef STRUCT_H
#define STRUCT_H

#include "common.h"

// 1) 사용자
typedef struct
{
    wchar_t user_id[32];    // 로그인 ID 겸 전역 키
    wchar_t password[32];
    wchar_t name[32];
} User;

// 2) 캘린더
typedef struct
{
    int     calendar_id;
    wchar_t user_id[32];    // 소유자 (User.user_id)
    wchar_t name[32];
    int     is_default;
} Calendar;

// 3) 카테고리, 반복
typedef enum
{
    CAT_NONE = 0,
    CAT_WORK,
    CAT_PERSONAL,
    CAT_HEALTH,
    CAT_SOCIAL,
    CAT_ETC
} Category;

typedef enum
{
    REPEAT_NONE = 0,
    REPEAT_DAILY,
    REPEAT_WEEKLY,
    REPEAT_MONTHLY
} RepeatType;

// 4) 일정(Schedule)
typedef struct
{
    int       schedule_id;
    int       calendar_id;

    wchar_t   title[64];
    wchar_t   location[64];
    wchar_t   memo[256];

    struct tm start_time;
    struct tm end_time;

    int       is_all_day;
    RepeatType repeat_type;
    Category  category;
} Schedule;

// 5) 공유 캘린더
typedef enum
{
    CAL_PERMISSION_VIEW = 0,
    CAL_PERMISSION_EDIT
} CalendarPermission;

typedef struct
{
    int     calendar_id;
    wchar_t owner_user_id[32];
    wchar_t shared_user_id[32];
    CalendarPermission perm;
} SharedCalendar;

// 6) 알림
typedef struct
{
    int  alarm_id;
    int  schedule_id;
    int  minutes_before;
    int  is_enabled;
} Alarm;

// 전역으로 들고 다니는 현재 로그인 사용자
extern User g_current_user;
#endif