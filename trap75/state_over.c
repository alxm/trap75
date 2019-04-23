/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of Trap75, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "state_over.h"

#include "obj_game.h"
#include "util_save.h"
#include "util_swipe.h"

void s_over_init(void)
{
    z_state_set(Z_STATE_TITLE);
    z_swipe_start(Z_SWIPE_FADE_HIDE);
}

void s_over_tick(void)
{
    n_game_tick();
}

void s_over_draw(void)
{
    n_game_draw();
}

void s_over_free(void)
{
    z_save_hiscoreSet(n_game_scoreGet());
}
