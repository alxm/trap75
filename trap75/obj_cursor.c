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

#include "obj_cursor.h"

#include "data_assets.h"
#include "obj_ball.h"
#include "obj_camera.h"
#include "obj_game.h"
#include "obj_map.h"
#include "state_play.h"
#include "util_color.h"
#include "util_input.h"

#define N_CURSOR_LINE_SPEED 2
#define N_CURSOR_SPEED (F_FIX_ONE / 1)
#define Z_HIT_TIMEOUT_MS 750
#define N_CURSOR_TRAIL_ALPHA 64
#define N_CURSOR_HISTORY_LEN 8

typedef enum {
    Z_LINE_INVALID = -1,
    Z_LINE_H,
    Z_LINE_V,
    Z_LINE_NUM
} ZLineState;

typedef struct {
    FVecFix coords;
    FVecInt coordsHistory[N_CURSOR_HISTORY_LEN];
    ZLineState line;
    int offsets[2];
    FTimer* timer;
} NCursor;

static NCursor g_cursor;

void n_cursor_new(void)
{
    g_cursor.coords.x = f_fix_fromInt(F_CONFIG_SCREEN_SIZE_WIDTH / 2);
    g_cursor.coords.y = f_fix_fromInt(F_CONFIG_SCREEN_SIZE_HEIGHT / 2);

    g_cursor.coordsHistory[0] = f_vecfix_toInt(g_cursor.coords);

    for(int i = 1; i < N_CURSOR_HISTORY_LEN; i++) {
        g_cursor.coordsHistory[i] = g_cursor.coordsHistory[0];
    }

    g_cursor.line = Z_LINE_INVALID;

    g_cursor.offsets[0] = 0;
    g_cursor.offsets[1] = 0;

    if(g_cursor.timer == NULL) {
        g_cursor.timer = f_timer_new(Z_HIT_TIMEOUT_MS, false);
    }

    f_timer_runStop(g_cursor.timer);
}

void n_cursor_tick(void)
{
    if(f_state_currentGet() != t_play) {
        return;
    }

    if(f_timer_runGet(g_cursor.timer)) {
        return;
    }

    if(f_timer_expiredGet(g_cursor.timer)) {
        g_cursor.line = Z_LINE_INVALID;
    }

    for(int i = N_CURSOR_HISTORY_LEN - 1; i--; ) {
        g_cursor.coordsHistory[i + 1] = g_cursor.coordsHistory[i];
    }

    g_cursor.coordsHistory[0] = f_vecfix_toInt(g_cursor.coords);

    if(g_cursor.line == Z_LINE_INVALID) {
        if(f_button_pressGet(u_input_get(U_BUTTON_UP))) {
            g_cursor.coords.y = f_math_max(g_cursor.coords.y - N_CURSOR_SPEED,
                                           F_FIX_ONE);
        }

        if(f_button_pressGet(u_input_get(U_BUTTON_DOWN))) {
            g_cursor.coords.y =
                f_math_min(g_cursor.coords.y + N_CURSOR_SPEED,
                           (F_CONFIG_SCREEN_SIZE_HEIGHT - 1) * F_FIX_ONE - 1);
        }

        if(f_button_pressGet(u_input_get(U_BUTTON_LEFT))) {
            g_cursor.coords.x = f_math_max(g_cursor.coords.x - N_CURSOR_SPEED,
                                           F_FIX_ONE);
        }

        if(f_button_pressGet(u_input_get(U_BUTTON_RIGHT))) {
            g_cursor.coords.x =
                f_math_min(g_cursor.coords.x + N_CURSOR_SPEED,
                           (F_CONFIG_SCREEN_SIZE_WIDTH - 1) * F_FIX_ONE - 1);
        }

        if(!n_map_test(f_vecfix_toInt(g_cursor.coords))) {
            if(f_button_pressGetOnce(u_input_get(U_BUTTON_A))) {
                g_cursor.line = Z_LINE_H;
                g_cursor.offsets[0] = 0;
                g_cursor.offsets[1] = 0;
            } else if(f_button_pressGetOnce(u_input_get(U_BUTTON_B))) {
                g_cursor.line = Z_LINE_V;
                g_cursor.offsets[0] = 0;
                g_cursor.offsets[1] = 0;
            }
        }
    } else {
        FVecInt origin = f_vecfix_toInt(g_cursor.coords);
        bool wall[2] = {false, false};

        int incs[Z_LINE_NUM][2][2] = {
            [Z_LINE_H] = {{-1, 0}, {1, 0}},
            [Z_LINE_V] = {{0, -1}, {0, 1}},
        };

        for(int w = 0; w < 2; w++) {
            for(int i = N_CURSOR_LINE_SPEED; i--; ) {
                FVecInt coords = {
                    origin.x + incs[g_cursor.line][w][0] * g_cursor.offsets[w],
                    origin.y + incs[g_cursor.line][w][1] * g_cursor.offsets[w]
                };

                if(n_map_test(coords)) {
                    wall[w] = true;

                    break;
                }

                g_cursor.offsets[w]++;
            }
        }

        bool hits;

        if(g_cursor.line == Z_LINE_H) {
            hits = o_ball_checkArea2(
                    g_cursor.coords.x - f_fix_fromInt(g_cursor.offsets[0] - 1),
                    g_cursor.coords.y,
                    f_fix_fromInt(
                        g_cursor.offsets[0] + 1 + g_cursor.offsets[1] - 2),
                    F_FIX_ONE);
        } else {
            hits = o_ball_checkArea2(
                    g_cursor.coords.x,
                    g_cursor.coords.y - f_fix_fromInt(g_cursor.offsets[0] - 1),
                    F_FIX_ONE,
                    f_fix_fromInt(
                        g_cursor.offsets[0] + 1 + g_cursor.offsets[1] - 2));
        }

        if(hits) {
            n_game_livesDec();
            n_camera_shakeSet(Z_HIT_TIMEOUT_MS);
            f_timer_runStart(g_cursor.timer);
        } else if(wall[0] && wall[1]) {
            FVecInt start[2];
            FVecInt dim[2];

            if(g_cursor.line == Z_LINE_H) {
                n_map_boundsGet(origin, 0, -1, &start[0], &dim[0]);
                n_map_boundsGet(origin, 0, 1, &start[1], &dim[1]);
            } else {
                n_map_boundsGet(origin, -1, 0, &start[0], &dim[0]);
                n_map_boundsGet(origin, 1, 0, &start[1], &dim[1]);
            }

            unsigned area;

            if(!o_ball_checkArea(start[0], dim[0])) {
                area = n_map_fill(
                        start[0].x, start[0].y, dim[0].x, dim[0].y);
            } else if(!o_ball_checkArea(start[1], dim[1])) {
                area = n_map_fill(
                        start[1].x, start[1].y, dim[1].x, dim[1].y);
            } else if(g_cursor.line == Z_LINE_H) {
                area = n_map_fill(
                        origin.x - g_cursor.offsets[0] + 1,
                        origin.y,
                        g_cursor.offsets[0] + 1 + g_cursor.offsets[1] - 2,
                        1);
            } else {
                area = n_map_fill(
                        origin.x,
                        origin.y - g_cursor.offsets[0] + 1,
                        1,
                        g_cursor.offsets[0] + 1 + g_cursor.offsets[1] - 2);
            }

            n_game_scoreAdd(area);

            g_cursor.line = Z_LINE_INVALID;
        }
    }
}

void n_cursor_draw(void)
{
    f_align_set(F_ALIGN_X_CENTER, F_ALIGN_Y_CENTER);
    f_color_blendSet(F_COLOR_BLEND_ALPHA_MASK);
    f_color_colorSetIndex(U_COLOR_CURSOR_TRAIL);

    for(int i = N_CURSOR_HISTORY_LEN; i--; ) {
        f_color_alphaSet(
            N_CURSOR_TRAIL_ALPHA
                - N_CURSOR_TRAIL_ALPHA * i / N_CURSOR_HISTORY_LEN);

        f_sprite_blit(f_gfx_assets_gfx_cursor_png,
                      0,
                      g_cursor.coordsHistory[i].x,
                      g_cursor.coordsHistory[i].y);
    }

    UColorId colorLine = U_COLOR_CURSOR_TRAIL;
    UColorId colorLineGlow = U_COLOR_CURSOR_MAIN;
    UColorId colorCursor = U_COLOR_CURSOR_MAIN;
    FVecInt shake = n_camera_shakeGet();
    FVecInt coords = f_vecfix_toInt(g_cursor.coords);

    if(f_timer_runGet(g_cursor.timer)) {
        if(f_fps_ticksGet() & 0x8) {
            colorLine = U_COLOR_BG_RED_2;
            colorLineGlow = U_COLOR_BG_RED_4;
        }
    } else {
        if(f_fps_ticksGet() & 0x4) {
            colorLine = U_COLOR_CURSOR_MAIN;
        }
    }

    if(g_cursor.line == Z_LINE_H) {
        f_color_colorSetIndex(colorLineGlow);
        f_color_alphaSet(64);

        // Left glow
        f_draw_rectangle(coords.x - g_cursor.offsets[0] - shake.x,
                         coords.y - 1 - shake.y,
                         g_cursor.offsets[0],
                         1);
        f_draw_rectangle(coords.x - g_cursor.offsets[0] - shake.x,
                         coords.y + 1 - shake.y,
                         g_cursor.offsets[0],
                         1);
        f_draw_pixel(coords.x - g_cursor.offsets[0] - 1 - shake.x,
                     coords.y - shake.y);

        // Right glow
        f_draw_rectangle(coords.x + 1 - shake.x,
                         coords.y - 1 - shake.y,
                         g_cursor.offsets[1],
                         1);
        f_draw_rectangle(coords.x + 1 - shake.x,
                         coords.y + 1 - shake.y,
                         g_cursor.offsets[1],
                         1);
        f_draw_pixel(coords.x + g_cursor.offsets[1] + 1 - shake.x,
                     coords.y - shake.y);

        // Bookends
        f_color_alphaSet(32);

        f_draw_pixel(coords.x - g_cursor.offsets[0] - 1 - shake.x,
                     coords.y - 1 - shake.y);
        f_draw_pixel(coords.x - g_cursor.offsets[0] - 1 - shake.x,
                     coords.y + 1 - shake.y);
        f_draw_pixel(coords.x + g_cursor.offsets[1] + 1 - shake.x,
                     coords.y - 1 - shake.y);
        f_draw_pixel(coords.x + g_cursor.offsets[1] + 1 - shake.x,
                     coords.y + 1 - shake.y);

        // Main
        f_color_blendSet(F_COLOR_BLEND_SOLID);
        f_color_colorSetIndex(colorLine);

        f_draw_rectangle(coords.x - g_cursor.offsets[0] - shake.x,
                         coords.y - shake.y,
                         g_cursor.offsets[0],
                         1);
        f_draw_rectangle(coords.x + 1 - shake.x,
                         coords.y - shake.y,
                         g_cursor.offsets[1],
                         1);
    } else if(g_cursor.line == Z_LINE_V) {
        f_color_colorSetIndex(colorLineGlow);
        f_color_alphaSet(64);

        // Up glow
        f_draw_rectangle(coords.x - 1 - shake.x,
                         coords.y - g_cursor.offsets[0] - shake.y,
                         1,
                         g_cursor.offsets[0]);
        f_draw_rectangle(coords.x + 1 - shake.x,
                         coords.y - g_cursor.offsets[0] - shake.y,
                         1,
                         g_cursor.offsets[0]);
        f_draw_pixel(coords.x, coords.y - g_cursor.offsets[0] - 1);

        // Down glow
        f_draw_rectangle(coords.x - 1 - shake.x,
                         coords.y + 1 - shake.y,
                         1,
                         g_cursor.offsets[1]);
        f_draw_rectangle(coords.x + 1 - shake.x,
                         coords.y + 1 - shake.y,
                         1,
                         g_cursor.offsets[1]);
        f_draw_pixel(coords.x - shake.x,
                     coords.y + g_cursor.offsets[1] + 1 - shake.y);

        // Bookends
        f_color_alphaSet(32);

        f_draw_pixel(coords.x - 1 - shake.x,
                     coords.y - g_cursor.offsets[0] - 1 - shake.y);
        f_draw_pixel(coords.x + 1 - shake.x,
                     coords.y - g_cursor.offsets[0] - 1 - shake.y);
        f_draw_pixel(coords.x - 1 - shake.x,
                     coords.y + g_cursor.offsets[1] + 1 - shake.y);
        f_draw_pixel(coords.x + 1 - shake.x,
                     coords.y + g_cursor.offsets[1] + 1 - shake.y);

        // Main
        f_color_blendSet(F_COLOR_BLEND_SOLID);
        f_color_colorSetIndex(colorLine);

        f_draw_rectangle(coords.x - shake.x,
                         coords.y - g_cursor.offsets[0] - shake.y,
                         1,
                         g_cursor.offsets[0]);
        f_draw_rectangle(coords.x - shake.x,
                         coords.y + 1 - shake.y,
                         1,
                         g_cursor.offsets[1]);
    }

    f_color_blendSet(F_COLOR_BLEND_ALPHA_MASK);
    f_color_colorSetIndex(colorCursor);
    f_color_alphaSet(F_COLOR_ALPHA_MAX);

    f_sprite_blit(
        f_gfx_assets_gfx_cursor_png, 0, coords.x + shake.x, coords.y + shake.x);
}
