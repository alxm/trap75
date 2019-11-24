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
#include "state_start.h"
#include "util_color.h"
#include "util_input.h"
#include "util_light.h"
#include "util_save.h"

void t_title(void)
{
    F_STATE_INIT
    {
        u_input_reset();

        n_map_new();

        f_color_colorSetIndex(U_COLOR_BG_PURPLE_1);
        f_fade_startColorFrom(500);
    }

    F_STATE_TICK
    {
        if(f_state_currentChanged()) {
            return;
        }

        if(u_input_any()) {
            u_light_pulseSet(U_LIGHT_GAME_START);

            f_color_colorSetIndex(U_COLOR_BG_PURPLE_1);
            f_fade_startColorTo(500);

            f_state_blockSet(f_fade_eventGet());
            f_state_replace(t_start);

            n_game_new();
        }
    }

    F_STATE_DRAW
    {
        n_map_draw();

        f_sprite_alignSet(F_SPRITE_ALIGN_X_CENTER | F_SPRITE_ALIGN_Y_TOP);

        #define Z_ALPHA_MIN 64
        #define Z_ALPHA_MAX 192
        #define Z_ALPHA_SWAY ((Z_ALPHA_MAX - Z_ALPHA_MIN) / 2)
        #define Z_ALPHA_BASELINE (Z_ALPHA_MIN + Z_ALPHA_SWAY)

        f_color_blendSet(F_COLOR_BLEND_ALPHA_MASK);
        f_color_colorSetIndex(U_COLOR_BALL_YELLOW_1);

        f_color_alphaSet(
            Z_ALPHA_BASELINE
                + f_fix_toInt(f_fps_ticksSin(1, 4, 0) * Z_ALPHA_SWAY));

        f_sprite_blit(f_gfx_assets_gfx_title_glow_png,
                      0,
                      F_CONFIG_SCREEN_SIZE_WIDTH / 2, 6);

        #undef Z_ALPHA_MIN
        #undef Z_ALPHA_MAX
        #define Z_ALPHA_MIN 192
        #define Z_ALPHA_MAX F_COLOR_ALPHA_MAX

        f_color_blendSet(F_COLOR_BLEND_ALPHA);

        f_color_alphaSet(
            Z_ALPHA_BASELINE
                + f_fix_toInt(f_fps_ticksSin(1, 2, 0) * Z_ALPHA_SWAY));

        f_sprite_blit(f_gfx_assets_gfx_title_png,
                      0,
                      F_CONFIG_SCREEN_SIZE_WIDTH / 2,
                      10);

        f_color_blendSet(F_COLOR_BLEND_PLAIN);

        f_sprite_blit(f_gfx_assets_gfx_alxm_footer_png,
                      0,
                      F_CONFIG_SCREEN_SIZE_WIDTH / 2,
                      53);

        f_color_blendSet(F_COLOR_BLEND_ALPHA_MASK);
        f_color_alphaSet(F_COLOR_ALPHA_MAX);

        f_font_fontSet(f_gfx_assets_gfx_font_aa_4x5_png);

        if(f_fps_ticksGet() & 0x28) {
            f_font_alignSet(F_FONT_ALIGN_MIDDLE);
            f_font_coordsSet(F_CONFIG_SCREEN_SIZE_WIDTH / 2, 38);

            f_font_print("Press Any Key");
        }

        f_font_alignSet(F_FONT_ALIGN_LEFT);
        f_color_colorSetIndex(U_COLOR_BALL_YELLOW_2);
        f_font_coordsSet(14, 31);
        f_font_print("Hiscore");

        f_font_alignSet(F_FONT_ALIGN_RIGHT);
        f_color_colorSetIndex(U_COLOR_BALL_YELLOW_3);
        f_font_coordsSet(66, 31);
        f_font_printf("%0*u", 5, u_save_hiscoreGet());
    }
}
