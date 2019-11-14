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

#include "util_timer.h"

typedef enum {
    Z_EXPIRED = F_FLAGS_BIT(0),
    Z_REPEAT = F_FLAGS_BIT(1),
    Z_RUNNING = F_FLAGS_BIT(2),
} ZTimerFlags;

typedef struct {
    unsigned base;
    unsigned period;
    unsigned flags;
} ZTimer;

static ZTimer g_timers[Z_TIMER_NUM];

void z_timer_tick(void)
{
    unsigned now = f_fps_ticksGet();

    for(int t = 0; t < Z_TIMER_NUM; t++) {
        ZTimer* timer = &g_timers[t];

        if(!F_FLAGS_TEST_ANY(timer->flags, Z_RUNNING)) {
            F_FLAGS_CLEAR(timer->flags, Z_EXPIRED);
            continue;
        }

        if(now - timer->base >= timer->period) {
            F_FLAGS_SET(timer->flags, Z_EXPIRED);

            if(F_FLAGS_TEST_ANY(timer->flags, Z_REPEAT)) {
                timer->base = now;
            } else {
                F_FLAGS_CLEAR(timer->flags, Z_RUNNING);
            }
        } else {
            F_FLAGS_CLEAR(timer->flags, Z_EXPIRED);
        }
    }
}

void z_timer_start(ZTimerId Timer, unsigned Ms, bool Repeat)
{
    ZTimer* timer = &g_timers[Timer];

    timer->base = f_fps_ticksGet();
    timer->period = f_time_ticksFromMs(Ms);
    timer->flags = Z_RUNNING;

    if(Ms == 0) {
        F_FLAGS_SET(timer->flags, Z_EXPIRED);
    }

    if(Repeat) {
        F_FLAGS_SET(timer->flags, Z_REPEAT);
    }
}

void z_timer_stop(ZTimerId Timer)
{
    ZTimer* timer = &g_timers[Timer];

    F_FLAGS_CLEAR(timer->flags, Z_RUNNING | Z_EXPIRED);
}

bool z_timer_isRunning(ZTimerId Timer)
{
    return F_FLAGS_TEST_ANY(g_timers[Timer].flags, Z_RUNNING);
}

bool z_timer_isExpired(ZTimerId Timer)
{
    return F_FLAGS_TEST_ANY(g_timers[Timer].flags, Z_EXPIRED);
}
