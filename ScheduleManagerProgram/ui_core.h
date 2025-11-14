// ui_core.h
#ifndef UI_CORE_H
#define UI_CORE_H

#include "common.h"

typedef struct {
    int x;
    int y;
} UiPoint;

typedef enum {
    UI_INPUT_NONE = 0,
    UI_INPUT_MOUSE_LEFT,
    UI_INPUT_MOUSE_RIGHT,
    UI_INPUT_KEY
} UiInputType;

typedef struct {
    UiInputType type;
    UiPoint     pos;    // 마우스일 때만 의미 있음
    wchar_t     key;    // 키보드일 때만 의미 (예: L'\n', L'1')
} UiInputEvent;

void Ui_InitConsole(void);
//void Ui_ClearScreen(void);
void DrawBox(int width, int height);
void set_cursor_visibility(int isVisible);
void goto_xy(int x, int y);
// 여기서 윈도우 콘솔 이벤트를 읽어오는 공통 함수
int Ui_WaitInput(UiInputEvent* out);

#endif