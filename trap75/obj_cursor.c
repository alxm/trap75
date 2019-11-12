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

#include "obj_ball.h"
#include "obj_camera.h"
#include "obj_game.h"
#include "obj_map.h"
#include "util_fix.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_state.h"
#include "util_timer.h"

#define N_CURSOR_LINE_SPEED 2
#define N_CURSOR_SPEED (Z_FIX_ONE / 1)
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
    ZVectorFix coords;
    ZVectorInt coordsHistory[N_CURSOR_HISTORY_LEN];
    ZLineState line;
    int offsets[2];
} NCursor;

static NCursor g_cursor;

void n_cursor_new(void)
{
    g_cursor.coords.x = z_fix_fromInt(Z_SCREEN_W / 2);
    g_cursor.coords.y = z_fix_fromInt(Z_SCREEN_H / 2);

    g_cursor.coordsHistory[0] = z_vectorfix_toInt(g_cursor.coords);

    for(int i = 1; i < N_CURSOR_HISTORY_LEN; i++) {
        g_cursor.coordsHistory[i] = g_cursor.coordsHistory[0];
    }

    g_cursor.line = Z_LINE_INVALID;

    g_cursor.offsets[0] = 0;
    g_cursor.offsets[1] = 0;

    z_timer_stop(Z_TIMER_LINE_HIT);
}

void n_cursor_tick(void)
{
    if(z_state_getCurrent() != Z_STATE_PLAY) {
        return;
    }

    if(z_timer_isRunning(Z_TIMER_LINE_HIT)) {
        return;
    }

    if(z_timer_isExpired(Z_TIMER_LINE_HIT)) {
        g_cursor.line = Z_LINE_INVALID;
    }

    for(int i = N_CURSOR_HISTORY_LEN - 1; i--; ) {
        g_cursor.coordsHistory[i + 1] = g_cursor.coordsHistory[i];
    }

    g_cursor.coordsHistory[0] = z_vectorfix_toInt(g_cursor.coords);

    if(g_cursor.line == Z_LINE_INVALID) {
        if(f_button_pressGet(u_input_get(U_BUTTON_UP))) {
            g_cursor.coords.y = z_math_max(g_cursor.coords.y - N_CURSOR_SPEED,
                                           Z_FIX_ONE);
        }

        if(f_button_pressGet(u_input_get(U_BUTTON_DOWN))) {
            g_cursor.coords.y = z_math_min(g_cursor.coords.y + N_CURSOR_SPEED,
                                           (Z_SCREEN_H - 1) * Z_FIX_ONE - 1);
        }

        if(f_button_pressGet(u_input_get(U_BUTTON_LEFT))) {
            g_cursor.coords.x = z_math_max(g_cursor.coords.x - N_CURSOR_SPEED,
                                           Z_FIX_ONE);
        }

        if(f_button_pressGet(u_input_get(U_BUTTON_RIGHT))) {
            g_cursor.coords.x = z_math_min(g_cursor.coords.x + N_CURSOR_SPEED,
                                           (Z_SCREEN_W - 1) * Z_FIX_ONE - 1);
        }

        if(!n_map_wallGet(z_vectorfix_toInt(g_cursor.coords))) {
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
        ZVectorInt origin = z_vectorfix_toInt(g_cursor.coords);
        bool wall[2] = {false, false};

        int incs[Z_LINE_NUM][2][2] = {
            [Z_LINE_H] = {{-1, 0}, {1, 0}},
            [Z_LINE_V] = {{0, -1}, {0, 1}},
        };

        for(int w = 0; w < 2; w++) {
            for(int i = N_CURSOR_LINE_SPEED; i--; ) {
                int wx = origin.x
                            + incs[g_cursor.line][w][0] * g_cursor.offsets[w];
                int wy = origin.y
                            + incs[g_cursor.line][w][1] * g_cursor.offsets[w];

                if(n_map_wallGet2(wx, wy)) {
                    wall[w] = true;
                    break;
                }

                g_cursor.offsets[w]++;
            }
        }

        bool hits;

        if(g_cursor.line == Z_LINE_H) {
            hits = o_ball_checkArea2(
                    g_cursor.coords.x - z_fix_fromInt(g_cursor.offsets[0] - 1),
                    g_cursor.coords.y,
                    z_fix_fromInt(
                        g_cursor.offsets[0] + 1 + g_cursor.offsets[1] - 2),
                    Z_FIX_ONE);
        } else {
            hits = o_ball_checkArea2(
                    g_cursor.coords.x,
                    g_cursor.coords.y - z_fix_fromInt(g_cursor.offsets[0] - 1),
                    Z_FIX_ONE,
                    z_fix_fromInt(
                        g_cursor.offsets[0] + 1 + g_cursor.offsets[1] - 2));
        }

        if(hits) {
            n_game_livesDec();
            n_camera_shakeSet(Z_HIT_TIMEOUT_MS);
            z_timer_start(Z_TIMER_LINE_HIT, Z_HIT_TIMEOUT_MS, false);
        } else if(wall[0] && wall[1]) {
            ZVectorInt start[2];
            ZVectorInt dim[2];

            if(g_cursor.line == Z_LINE_H) {
                n_map_wallBoundsGet(origin, 0, -1, &start[0], &dim[0]);
                n_map_wallBoundsGet(origin, 0, 1, &start[1], &dim[1]);
            } else {
                n_map_wallBoundsGet(origin, -1, 0, &start[0], &dim[0]);
                n_map_wallBoundsGet(origin, 1, 0, &start[1], &dim[1]);
            }

            unsigned area;

            if(!o_ball_checkArea(start[0], dim[0])) {
                area = n_map_wallFill(
                        start[0].x, start[0].y, dim[0].x, dim[0].y);
            } else if(!o_ball_checkArea(start[1], dim[1])) {
                area = n_map_wallFill(
                        start[1].x, start[1].y, dim[1].x, dim[1].y);
            } else if(g_cursor.line == Z_LINE_H) {
                area = n_map_wallFill(
                        origin.x - g_cursor.offsets[0] + 1,
                        origin.y,
                        g_cursor.offsets[0] + 1 + g_cursor.offsets[1] - 2,
                        1);
            } else {
                area = n_map_wallFill(
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
    z_graphics_colorSetId(Z_COLOR_CURSOR_TRAIL);
    z_sprite_align(Z_ALIGN_X_CENTER | Z_ALIGN_Y_CENTER);

    for(int i = N_CURSOR_HISTORY_LEN; i--; ) {
        z_graphics_alphaSet(
            N_CURSOR_TRAIL_ALPHA
                - N_CURSOR_TRAIL_ALPHA * i / N_CURSOR_HISTORY_LEN);

        z_sprite_blitAlphaMask(Z_SPRITE_CURSOR,
                               0,
                               g_cursor.coordsHistory[i].x,
                               g_cursor.coordsHistory[i].y);
    }

    ZColorId colorLine = Z_COLOR_CURSOR_TRAIL;
    ZColorId colorLineGlow = Z_COLOR_CURSOR_MAIN;
    ZColorId colorCursor = Z_COLOR_CURSOR_MAIN;
    ZVectorInt shake = n_camera_shakeGet();
    ZVectorInt coords = z_vectorfix_toInt(g_cursor.coords);

    if(z_timer_isRunning(Z_TIMER_LINE_HIT)) {
        if(z_fps_ticksGet() & 0x8) {
            colorLine = Z_COLOR_BG_RED_2;
            colorLineGlow = Z_COLOR_BG_RED_4;
        }
    } else {
        if(z_fps_ticksGet() & 0x4) {
            colorLine = Z_COLOR_CURSOR_MAIN;
        }
    }

    if(g_cursor.line == Z_LINE_H) {
        z_graphics_colorSetId(colorLineGlow);
        z_graphics_alphaSet(64);

        // Left glow
        z_draw_rectangleAlpha(coords.x - g_cursor.offsets[0] - shake.x,
                              coords.y - 1 - shake.y,
                              g_cursor.offsets[0],
                              1);
        z_draw_rectangleAlpha(coords.x - g_cursor.offsets[0] - shake.x,
                              coords.y + 1 - shake.y,
                              g_cursor.offsets[0],
                              1);
        z_draw_pixelAlpha(
            coords.x - g_cursor.offsets[0] - 1 - shake.x, coords.y - shake.y);

        // Right glow
        z_draw_rectangleAlpha(coords.x + 1 - shake.x,
                              coords.y - 1 - shake.y,
                              g_cursor.offsets[1],
                              1);
        z_draw_rectangleAlpha(coords.x + 1 - shake.x,
                              coords.y + 1 - shake.y,
                              g_cursor.offsets[1],
                              1);
        z_draw_pixelAlpha(
            coords.x + g_cursor.offsets[1] + 1 - shake.x, coords.y - shake.y);

        // Bookends
        z_graphics_alphaSet(32);

        z_draw_pixelAlpha(coords.x - g_cursor.offsets[0] - 1 - shake.x,
                          coords.y - 1 - shake.y);
        z_draw_pixelAlpha(coords.x - g_cursor.offsets[0] - 1 - shake.x,
                          coords.y + 1 - shake.y);
        z_draw_pixelAlpha(coords.x + g_cursor.offsets[1] + 1 - shake.x,
                          coords.y - 1 - shake.y);
        z_draw_pixelAlpha(coords.x + g_cursor.offsets[1] + 1 - shake.x,
                          coords.y + 1 - shake.y);

        // Main
        z_graphics_colorSetId(colorLine);

        z_draw_rectangle(coords.x - g_cursor.offsets[0] - shake.x,
                         coords.y - shake.y,
                         g_cursor.offsets[0],
                         1);
        z_draw_rectangle(coords.x + 1 - shake.x,
                         coords.y - shake.y,
                         g_cursor.offsets[1],
                         1);
    } else if(g_cursor.line == Z_LINE_V) {
        z_graphics_colorSetId(colorLineGlow);
        z_graphics_alphaSet(64);

        // Up glow
        z_draw_rectangleAlpha(coords.x - 1 - shake.x,
                              coords.y - g_cursor.offsets[0] - shake.y,
                              1,
                              g_cursor.offsets[0]);
        z_draw_rectangleAlpha(coords.x + 1 - shake.x,
                              coords.y - g_cursor.offsets[0] - shake.y,
                              1,
                              g_cursor.offsets[0]);
        z_draw_pixelAlpha(coords.x, coords.y - g_cursor.offsets[0] - 1);

        // Down glow
        z_draw_rectangleAlpha(coords.x - 1 - shake.x,
                              coords.y + 1 - shake.y,
                              1,
                              g_cursor.offsets[1]);
        z_draw_rectangleAlpha(coords.x + 1 - shake.x,
                              coords.y + 1 - shake.y,
                              1,
                              g_cursor.offsets[1]);
        z_draw_pixelAlpha(
            coords.x - shake.x, coords.y + g_cursor.offsets[1] + 1 - shake.y);

        // Bookends
        z_graphics_alphaSet(32);

        z_draw_pixelAlpha(coords.x - 1 - shake.x,
                          coords.y - g_cursor.offsets[0] - 1 - shake.y);
        z_draw_pixelAlpha(coords.x + 1 - shake.x,
                          coords.y - g_cursor.offsets[0] - 1 - shake.y);
        z_draw_pixelAlpha(coords.x - 1 - shake.x,
                          coords.y + g_cursor.offsets[1] + 1 - shake.y);
        z_draw_pixelAlpha(coords.x + 1 - shake.x,
                          coords.y + g_cursor.offsets[1] + 1 - shake.y);

        // Main
        z_graphics_colorSetId(colorLine);

        z_draw_rectangle(coords.x - shake.x,
                         coords.y - g_cursor.offsets[0] - shake.y,
                         1,
                         g_cursor.offsets[0]);
        z_draw_rectangle(coords.x - shake.x,
                         coords.y + 1 - shake.y,
                         1,
                         g_cursor.offsets[1]);
    }

    z_graphics_colorSetId(colorCursor);
    z_graphics_alphaSet(256);

    z_sprite_blitAlphaMask(
        Z_SPRITE_CURSOR, 0, coords.x + shake.x, coords.y + shake.x);
}
