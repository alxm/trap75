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

#include "state_title.h"

#include "data_assets.h"
#include "obj_game.h"
#include "obj_map.h"
#include "util_graphics.h"
#include "util_font.h"
#include "util_input.h"
#include "util_light.h"
#include "util_save.h"
#include "util_swipe.h"

void s_title_init(void)
{
    u_input_reset();

    n_map_new();

    z_swipe_start(Z_SWIPE_FADE_SHOW);
}

void s_title_tick(void)
{
    if(z_state_changed()) {
        return;
    }

    if(u_input_any()) {
        z_state_set(Z_STATE_START);
        z_swipe_start(Z_SWIPE_FADE_HIDE);

        n_game_new();

        z_light_pulseSet(Z_LIGHT_GAME_START);
    }
}

void s_title_draw(void)
{
    n_map_draw();

    f_sprite_alignSet(F_SPRITE_ALIGN_X_CENTER | F_SPRITE_ALIGN_Y_TOP);

    f_color_blendSet(F_COLOR_BLEND_ALPHA_MASK);
    f_color_colorSetPixel(z_colors[Z_COLOR_BALL_YELLOW_1].pixel);

    #define Z_ALPHA_MIN 64
    #define Z_ALPHA_BASELINE \
        (Z_ALPHA_MIN + (F_COLOR_ALPHA_MAX - Z_ALPHA_MIN) / 2)

    f_color_alphaSet(
        Z_ALPHA_BASELINE
            + f_fix_toInt((Z_ALPHA_BASELINE - Z_ALPHA_MIN)
                            * f_fix_sin(f_fps_ticksGet() << 5)));

    f_sprite_blit(f_gfx_assets_gfx_title_glow_png,
                  0,
                  F_CONFIG_SCREEN_SIZE_WIDTH / 2, 6);

    f_color_blendSet(F_COLOR_BLEND_PLAIN);

    f_sprite_blit(f_gfx_assets_gfx_title_png,
                  0,
                  F_CONFIG_SCREEN_SIZE_WIDTH / 2,
                  10);

    f_sprite_blit(f_gfx_assets_gfx_alxm_footer_png,
                  0,
                  F_CONFIG_SCREEN_SIZE_WIDTH / 2,
                  53);

    f_color_blendSet(F_COLOR_BLEND_ALPHA_MASK);
    f_color_alphaSet(F_COLOR_ALPHA_MAX);

    if(f_fps_ticksGet() & 0x28) {
        z_font_align(F_SPRITE_ALIGN_X_CENTER | F_SPRITE_ALIGN_Y_TOP);
        z_font_printText(F_CONFIG_SCREEN_SIZE_WIDTH / 2, 38, "Press Any Key");
    }

    z_font_align(F_SPRITE_ALIGN_X_LEFT | F_SPRITE_ALIGN_Y_TOP);

    f_color_colorSetPixel(z_colors[Z_COLOR_BALL_YELLOW_2].pixel);
    z_font_printText(15, 31, "Hiscore");

    f_color_colorSetPixel(z_colors[Z_COLOR_BALL_YELLOW_3].pixel);
    z_font_printIntup(47, 31, z_save_hiscoreGet(), 5);
}
