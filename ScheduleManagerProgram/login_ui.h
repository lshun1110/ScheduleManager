#ifndef LOGIN_UI_H
#define LOGIN_UI_H

#include "common.h"


void draw_login_screen();
void print_title_segment();
void LoginUi_Draw(const wchar_t* id, const wchar_t* pw, int focus_is_id, int showPassword, int lastLoginFailed);

#endif