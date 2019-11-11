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

#include "state_over.h"

#include "obj_game.h"
#include "util_swipe.h"

void s_next_init(void)
{
    z_state_set(Z_STATE_START);
    z_swipe_start(Z_SWIPE_FADE_HIDE);
}

void s_next_tick(void)
{
    n_game_tick();
}

void s_next_draw(void)
{
    n_game_draw();
}

void s_next_free(void)
{
    n_game_levelSet(n_game_levelGet() + 1);
}
