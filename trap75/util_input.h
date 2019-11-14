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

#pragma once

#include <faur.h>

F_EXTERN_C_START

typedef enum {
    U_BUTTON_INVALID = -1,
    U_BUTTON_UP,
    U_BUTTON_DOWN,
    U_BUTTON_LEFT,
    U_BUTTON_RIGHT,
    U_BUTTON_A,
    U_BUTTON_B,
    U_BUTTON_MENU,
    U_BUTTON_NUM
} UButtonId;

extern void u_input_init(void);
extern void u_input_uninit(void);

extern FButton* u_input_get(UButtonId Button);
extern void u_input_reset(void);
extern bool u_input_any(void);

F_EXTERN_C_END
