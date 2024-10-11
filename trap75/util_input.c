/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of Trap75, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "util_input.h"

static FButton* g_buttons[U_BUTTON_NUM];

void u_input_init(void)
{
    g_buttons[U_BUTTON_UP] = f_button_new();
    f_button_bindKey(g_buttons[U_BUTTON_UP], F_KEY_UP);
    f_button_bindButton(g_buttons[U_BUTTON_UP], F_BUTTON_UP);

    g_buttons[U_BUTTON_DOWN] = f_button_new();
    f_button_bindKey(g_buttons[U_BUTTON_DOWN], F_KEY_DOWN);
    f_button_bindButton(g_buttons[U_BUTTON_DOWN], F_BUTTON_DOWN);

    g_buttons[U_BUTTON_LEFT] = f_button_new();
    f_button_bindKey(g_buttons[U_BUTTON_LEFT], F_KEY_LEFT);
    f_button_bindButton(g_buttons[U_BUTTON_LEFT], F_BUTTON_LEFT);
    f_button_bindButton(g_buttons[U_BUTTON_LEFT], F_BUTTON_L);

    g_buttons[U_BUTTON_RIGHT] = f_button_new();
    f_button_bindKey(g_buttons[U_BUTTON_RIGHT], F_KEY_RIGHT);
    f_button_bindButton(g_buttons[U_BUTTON_RIGHT], F_BUTTON_RIGHT);
    f_button_bindButton(g_buttons[U_BUTTON_RIGHT], F_BUTTON_R);

    g_buttons[U_BUTTON_A] = f_button_new();
    f_button_bindKey(g_buttons[U_BUTTON_A], F_KEY_SPACE);
    f_button_bindKey(g_buttons[U_BUTTON_A], F_KEY_Z);
    f_button_bindButton(g_buttons[U_BUTTON_A], F_BUTTON_A);

    g_buttons[U_BUTTON_B] = f_button_new();
    f_button_bindKey(g_buttons[U_BUTTON_B], F_KEY_X);
    f_button_bindButton(g_buttons[U_BUTTON_B], F_BUTTON_B);

    g_buttons[U_BUTTON_MENU] = f_button_new();
    f_button_bindKey(g_buttons[U_BUTTON_MENU], F_KEY_ENTER);
    f_button_bindButton(g_buttons[U_BUTTON_MENU], F_BUTTON_START);
}

void u_input_uninit(void)
{
    for(int b = 0; b < U_BUTTON_NUM; b++) {
        f_button_free(g_buttons[b]);
    }
}

FButton* u_input_get(UButtonId Button)
{
    return g_buttons[Button];
}

void u_input_reset(void)
{
    for(int b = 0; b < U_BUTTON_NUM; b++) {
        f_button_pressClear(g_buttons[b]);
    }
}

bool u_input_any(void)
{
    for(int b = 0; b < U_BUTTON_NUM; b++) {
        if(f_button_pressGet(g_buttons[b])) {
            return true;
        }
    }

    return false;
}
