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

#include <faur.h>

#include "platform.h"

#include "util_state.h"

static void t_run(void)
{
    F_STATE_INIT
    {
        z_platform_init();
        z_state_setup();
    }

    F_STATE_TICK
    {
        z_state_tick();
    }

    F_STATE_DRAW
    {
        z_state_draw();
    }

    F_STATE_FREE
    {
        z_platform_uninit();
    }
}

void f_main(void)
{
    f_state_push(t_run);
}
