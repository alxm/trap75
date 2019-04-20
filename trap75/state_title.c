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
#include "util_fps.h"
#include "util_input.h"
#include "util_swipe.h"

void s_title_init(void)
{
    z_input_reset();
    n_map_new();

    z_swipe_start(Z_SWIPE_FADE_SHOW);
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

    z_graphics_alphaSet(256);
    z_graphics_colorSetId(Z_COLOR_BALL_YELLOW_1);
    z_sprite_align(Z_ALIGN_X_CENTER | Z_ALIGN_Y_TOP);

    #define Z_ALPHA_MIN 64
    #define Z_ALPHA_BASELINE (Z_ALPHA_MIN + (256 - Z_ALPHA_MIN) / 2)

    z_graphics_alphaSet(
        Z_ALPHA_BASELINE
            + z_fix_toInt(
                (Z_ALPHA_BASELINE - Z_ALPHA_MIN) / 2
                    * z_fix_sin(z_fps_ticksGet() << 5)));

    z_sprite_blitAlphaMask(Z_SPRITE_TITLE_GLOW, 0, Z_SCREEN_W / 2, 6);
    z_sprite_blit(Z_SPRITE_TITLE, 0, Z_SCREEN_W / 2, 10);

    if(z_fps_ticksGet() & 0x28) {
        z_sprite_blitAlphaMask(Z_SPRITE_ICON_PRESS, 0, Z_SCREEN_W / 2, 34);
    }

    z_sprite_blit(Z_SPRITE_ALXM_FOOTER, 0, Z_SCREEN_W / 2, 53);
}
