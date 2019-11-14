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
#include "state_next.h"
#include "state_over.h"

void t_play(void)
{
    F_STATE_TICK
    {
        n_game_tick();

        if(n_game_livesGet() == 0) {
            f_state_replace(t_over);
        } else if(n_map_wallPercentGet() >= 75) {
            f_state_replace(t_next);
        }
    }

    F_STATE_DRAW
    {
        n_game_draw();
    }
}
