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
#include "util_input.h"
#include "util_swipe.h"

void s_start_init(void)
{
    z_input_reset();

    z_swipe_start(Z_SWIPE_FADE_SHOW);
}

void s_start_tick(void)
{
    if(z_button_pressGetAny()) {
        z_state_set(Z_STATE_PLAY);
    }

    n_game_tick();
}

void s_start_draw(void)
{
    n_game_draw();
}
