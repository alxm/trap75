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

#include "util_timer.h"

typedef struct {
    ZVectorInt shake;
} NCamera;

static NCamera g_camera;

void n_camera_new(void)
{
    g_camera.shake = (ZVectorInt){0, 0};

    z_timer_stop(Z_TIMER_CAMERA_SHAKE);
}

void n_camera_tick(void)
{
    if(z_timer_isRunning(Z_TIMER_CAMERA_SHAKE)) {
        g_camera.shake = (ZVectorInt){z_random_range(-1, 2),
                                      z_random_range(-1, 2)};
    } else {
        g_camera.shake = (ZVectorInt){0, 0};
    }
}

ZVectorInt n_camera_shakeGet(void)
{
    return g_camera.shake;
}

void n_camera_shakeSet(unsigned Ms)
{
    z_timer_start(Z_TIMER_CAMERA_SHAKE, Ms, false);
}
