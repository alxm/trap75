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

#include "util_graphics.h"

typedef enum {
    Z_LIGHT_INVALID = -1,
    Z_LIGHT_GAME_START,
    Z_LIGHT_NUM
} ZLightId;

extern void z_light_reset(void);

extern void z_light_tick(void);
extern void z_light_draw(void);

extern void z_light_pulseSet(ZLightId Light);
extern void z_light_backgroundSet(ZColorId Color);
