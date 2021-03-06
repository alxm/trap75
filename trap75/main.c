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

#include "state_intro.h"
#include "util_input.h"
#include "util_light.h"
#include "util_save.h"

void f_main(void)
{
    F_STATE_INIT
    {
        u_color_init();
        u_input_init();
        u_save_init();

        f_state_push(t_intro);
        f_state_callbacks(u_light_tick, NULL, NULL, u_light_draw);
    }

    F_STATE_FREE
    {
        u_input_uninit();
        u_color_uninit();
    }
}
