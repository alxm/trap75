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

typedef enum {
    Z_TIMER_INVALID = -1,
    Z_TIMER_CAMERA_SHAKE,
    Z_TIMER_G1,
    Z_TIMER_LINE_HIT,
    Z_TIMER_NUM
} ZTimerId;

extern void z_timer_tick(void);

extern void z_timer_start(ZTimerId Timer, unsigned Ms, bool Repeat);
extern void z_timer_stop(ZTimerId Timer);
extern bool z_timer_isRunning(ZTimerId Timer);
extern bool z_timer_isExpired(ZTimerId Timer);
