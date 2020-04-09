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

#include "obj_camera.h"

typedef struct {
    FVecInt shake;
    FTimer* timer;
} NCamera;

static NCamera g_camera;

void n_camera_new(void)
{
    g_camera.shake = (FVecInt){0, 0};

    if(g_camera.timer == NULL) {
        g_camera.timer = f_timer_new(F_TIMER_MS, 0, false);
    }

    f_timer_runStop(g_camera.timer);
}

void n_camera_tick(void)
{
    if(f_timer_runGet(g_camera.timer)) {
        g_camera.shake = (FVecInt){f_random_range(-1, 2),
                                   f_random_range(-1, 2)};
    } else {
        g_camera.shake = (FVecInt){0, 0};
    }
}

FVecInt n_camera_shakeGet(void)
{
    return g_camera.shake;
}

void n_camera_shakeSet(unsigned Ms)
{
    f_timer_periodSet(g_camera.timer, Ms);
    f_timer_runStart(g_camera.timer);
}
