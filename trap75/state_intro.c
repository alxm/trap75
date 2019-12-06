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

#include "state_intro.h"

#include "data_assets.h"
#include "state_title.h"
#include "util_color.h"
#include "util_input.h"

#define Z_LOGO_WAIT_MS 800

static const int8_t g_lines[] = {
    -1,
    8, 4, -1,
    8, 3, -1,
    7, 2, 6, 4, -1,
    7, 1, 6, 3, -1,
    6, 0, 5, 2, 4, 4, -1,
    5, 1, 4, 3, -1,
    4, 0, 3, 2, 2, 4, -1,
    3, 1, 2, 3, -1,
    2, 0, 1, 2, -1,
    1, 1, -1,
    0, 0, -1,
    -2
};

static int g_pc;

void t_intro(void)
{
    static FTimer* timer;

    F_STATE_INIT
    {
        f_color_colorSetIndex(U_COLOR_BG_PURPLE_1);
        f_screen_clear();

        timer = f_timer_new(F_TIMER_MS, Z_LOGO_WAIT_MS, false);
    }

    F_STATE_TICK
    {
        if(f_state_currentChanged()) {
            return;
        }

        if(f_timer_expiredGet(timer)
            || f_button_pressGetOnce(u_input_get(U_BUTTON_A))
            || f_button_pressGetOnce(u_input_get(U_BUTTON_B))) {

            f_color_colorSetIndex(U_COLOR_BG_PURPLE_1);
            f_fade_startColorTo(500);

            f_state_blockSet(f_fade_eventGet());
            f_state_replace(t_title);

            g_pc = F_ARRAY_LEN(g_lines) - 1;
        }

        if(f_state_currentChanged() || f_timer_isRunning(timer)) {
            return;
        }

        while(g_lines[g_pc] >= 0) {
            g_pc++;
        }

        if(g_lines[g_pc] == -1) {
            g_pc++;
        }

        if(g_lines[g_pc] == -2) {
            f_timer_start(timer);
        }
    }

    F_STATE_DRAW
    {
        f_color_blendSet(F_COLOR_BLEND_SOLID);
        f_color_colorSetIndex(U_COLOR_ALXM_BG);
        f_draw_fill();

        f_align_set(F_ALIGN_X_CENTER, F_ALIGN_Y_CENTER);
        f_sprite_blit(f_gfx_assets_gfx_alxm_png,
                      0,
                      F_CONFIG_SCREEN_SIZE_WIDTH / 2,
                      F_CONFIG_SCREEN_SIZE_HEIGHT / 2);

        if(g_lines[g_pc] < 0) {
            return;
        }

        FVectorInt logoSize = f_sprite_sizeGet(f_gfx_assets_gfx_alxm_png);
        int startX1 = F_CONFIG_SCREEN_SIZE_WIDTH / 2 - logoSize.x / 2;
        int startX2 = F_CONFIG_SCREEN_SIZE_WIDTH / 2 + (logoSize.x + 1) / 2 - 2;
        int startY = F_CONFIG_SCREEN_SIZE_HEIGHT / 2 - logoSize.y / 2;

        f_color_colorSetIndex(U_COLOR_ALXM_BG);

        for(int pc = g_pc; g_lines[pc] != -2; pc++) {
            while(g_lines[pc] != -1) {
                int8_t x = g_lines[pc++];
                int8_t y = g_lines[pc++];

                f_draw_rectangle(startX1 + x, startY + y, 2, 1);
                f_draw_rectangle(startX2 - x, startY + y, 2, 1);
            }
        }
    }

    F_STATE_FREE
    {
        f_timer_free(timer);
    }
}
