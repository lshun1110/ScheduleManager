#include "common.h"
#include "ui_core.h"

static HANDLE hConsole;
static HANDLE hInput;

void Ui_InitConsole(void)
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    hInput = GetStdHandle(STD_INPUT_HANDLE);

    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
}

int Ui_WaitInput(UiInputEvent* out)
{
    INPUT_RECORD rec;
    DWORD readCount;

    while (1) {
        ReadConsoleInputW(hInput, &rec, 1, &readCount);

        if (rec.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD m = rec.Event.MouseEvent;

            if (m.dwEventFlags == 0 && (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) {
                out->type = UI_INPUT_MOUSE_LEFT;
                out->pos.x = m.dwMousePosition.X;
                out->pos.y = m.dwMousePosition.Y;
                out->key = 0;
                return 1;
            }
        }
        else if (rec.EventType == KEY_EVENT) {
            KEY_EVENT_RECORD k = rec.Event.KeyEvent;

            if (k.bKeyDown) {
                out->type = UI_INPUT_KEY;
                out->pos.x = out->pos.y = -1;
                out->key = k.uChar.UnicodeChar;
                return 1;
            }
        }
    }

    // 도달할 일은 거의 없음
    return 0;
}


void draw_box(int x, int y, int width, int height)
{
    if (width < 2 || height < 2) {
        return;    // 최소 크기 체크
    }

    goto_xy(x, y);
    wprintf(L"┌");
    for (int i = 0; i < width - 2; i++)
        wprintf(L"─");
    wprintf(L"┐");

    for (int i = 1; i < height - 1; i++)
    {
        goto_xy(x, y + i);
        wprintf(L"│");

        goto_xy(x + width - 1, y + i);
        wprintf(L"│");
    }

    goto_xy(x, y + height - 1);
    wprintf(L"└");
    for (int i = 0; i < width - 2; i++)
        wprintf(L"─");
    wprintf(L"┘");
}

void set_cursor_visibility(int isVisible)
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = isVisible;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void goto_xy(int x, int y)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = (SHORT)x;
    pos.Y = (SHORT)y;

    if (!SetConsoleCursorPosition(hOut, pos)) {
        DWORD err = GetLastError();
        wprintf(L"[goto_xy 실패] x=%d, y=%d, error=%lu\n", x, y, err);
    }
}

int Ui_PointInRect(const UiRect* r, int x, int y)
{
    return (x >= r->x && x < r->x + r->w &&
        y >= r->y && y < r->y + r->h);
}