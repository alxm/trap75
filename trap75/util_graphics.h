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
    Z_COLOR_INVALID = -1,

    Z_COLOR_ALXM_BG,
    Z_COLOR_ALXM_Y1,
    Z_COLOR_ALXM_Y2,
    Z_COLOR_ALXM_G1,
    Z_COLOR_ALXM_G2,
    Z_COLOR_ALXM_B1,
    Z_COLOR_ALXM_B2,
    Z_COLOR_ALXM_R1,
    Z_COLOR_ALXM_R2,

    Z_COLOR_BG_PURPLE_1,
    Z_COLOR_BG_PURPLE_2,
    Z_COLOR_BG_GREEN_1,
    Z_COLOR_BG_GREEN_2,
    Z_COLOR_BG_RED_1,
    Z_COLOR_BG_RED_2,
    Z_COLOR_BG_RED_3,
    Z_COLOR_BG_RED_4,

    Z_COLOR_BALL_YELLOW_1,
    Z_COLOR_BALL_YELLOW_2,
    Z_COLOR_BALL_YELLOW_3,
    Z_COLOR_BALL_YELLOW_4,

    Z_COLOR_CURSOR_TRAIL,
    Z_COLOR_CURSOR_MAIN,

    Z_COLOR_NUM
} ZColorId;

typedef struct ZColor {
    FColorPixel pixel;
    FColorRgb rgb;
} ZColor;

extern ZColor z_colors[Z_COLOR_NUM];

extern void z_graphics_setup(void);

F_EXTERN_C_END
