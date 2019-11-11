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

#include "state_play.h"

#include "obj_game.h"
#include "obj_map.h"

void s_play_tick(void)
{
    n_game_tick();

    if(n_game_livesGet() == 0) {
        z_state_set(Z_STATE_OVER);
    } else if(n_map_wallPercentGet() >= 75) {
        z_state_set(Z_STATE_NEXT);
    }
}

void s_play_draw(void)
{
    n_game_draw();
}
