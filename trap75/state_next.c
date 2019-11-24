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

#include "state_next.h"

#include "obj_game.h"
#include "state_start.h"
#include "util_color.h"

void t_next(void)
{
    F_STATE_INIT
    {
        f_color_colorSetIndex(U_COLOR_BG_PURPLE_1);
        f_fade_startColorTo(500);

        f_state_blockSet(f_fade_eventGet());
        f_state_replace(t_start);
    }

    F_STATE_TICK
    {
        n_game_tick();
    }

    F_STATE_DRAW
    {
        n_game_draw();
    }

    F_STATE_FREE
    {
        n_game_levelSet(n_game_levelGet() + 1);
    }
}
