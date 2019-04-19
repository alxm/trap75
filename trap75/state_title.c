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

#include "state_title.h"

#include "obj_game.h"
#include "obj_map.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_swipe.h"

void s_title_init(void)
{
    z_input_reset();

    n_map_new();
    n_map_wallFill(
        N_MAP_BORDER_L, 30, Z_SCREEN_W - N_MAP_BORDER_L - N_MAP_BORDER_R, 18);

    z_swipe_start(Z_SWIPE_LINES_SHOW);
}

void s_title_tick(void)
{
    if(z_state_changed()) {
        return;
    }

    if(z_button_pressGetOnce(Z_BUTTON_A) || z_button_pressGetOnce(Z_BUTTON_B)) {
        z_state_set(Z_STATE_START);
        z_swipe_start(Z_SWIPE_FADE_HIDE);
    }
}

void s_title_draw(void)
{
    n_map_draw();

    z_sprite_align(Z_ALIGN_X_CENTER | Z_ALIGN_Y_TOP);
    z_sprite_blit(Z_SPRITE_TITLE, 0, Z_SCREEN_W / 2, 10);
    z_sprite_blit(Z_SPRITE_ALXM_FOOTER, 0, Z_SCREEN_W / 2, 53);
}
