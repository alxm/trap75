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

#include "state_start.h"

#include "obj_game.h"
#include "state_play.h"
#include "util_color.h"
#include "util_input.h"

void t_start(void)
{
    F_STATE_INIT
    {
        u_input_reset();

        f_color_colorSetPixel(u_colors[U_COLOR_BG_PURPLE_1].pixel);
        f_fade_startColorFrom(500);
    }

    F_STATE_TICK
    {
        if(u_input_any()) {
            f_state_replace(t_play);
        }

        n_game_tick();
    }

    F_STATE_DRAW
    {
        n_game_draw();
    }
}
