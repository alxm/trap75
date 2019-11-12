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

#include "util_swipe.h"

#include "data_assets.h"
#include "util_graphics.h"

#define Z_ANGLE_INC (F_DEG_090_FIX / 16)

typedef void (ZSwipeInit)(void);
typedef bool (ZSwipeTick)(void);
typedef void (ZSwipeDraw)(void);

static FFixu g_angle;
static ZSwipeId g_swipe = Z_SWIPE_INVALID;

static void drawFadeHide(void)
{
    z_graphics_colorSetId(Z_COLOR_BG_PURPLE_1);
    z_graphics_alphaSet(f_fix_toInt(f_fix_sinf(g_angle) * 256));

    z_draw_rectangleAlpha(0, 0, Z_SCREEN_W, Z_SCREEN_H);
}

static void drawFadeShow(void)
{
    z_graphics_colorSetId(Z_COLOR_BG_PURPLE_1);
    z_graphics_alphaSet(f_fix_toInt(f_fix_sinf(F_DEG_090_FIX - g_angle) * 256));

    z_draw_rectangleAlpha(0, 0, Z_SCREEN_W, Z_SCREEN_H);
}

static void drawLines(FFixu Angle)
{
    FFix sine = f_fix_sinf(Angle);
    int h = f_fix_toInt(sine * (Z_SCREEN_H / 2));
    int alpha = f_fix_toInt(sine * 256);

    z_graphics_colorSetId(Z_COLOR_BG_PURPLE_1);
    z_graphics_alphaSet(alpha);

    z_draw_rectangleAlpha(0, 0, Z_SCREEN_W, h);
    z_draw_rectangleAlpha(0, Z_SCREEN_H - h, Z_SCREEN_W, h);

    z_graphics_colorSetId(Z_COLOR_BG_PURPLE_2);

    z_draw_hline(0, Z_SCREEN_W - 1, h);
    z_draw_hline(0, Z_SCREEN_W - 1, Z_SCREEN_H - h - 1);
}

static void drawLinesHide(void)
{
    drawLines(g_angle);
}

static void drawLinesShow(void)
{
    drawLines(F_DEG_090_FIX - g_angle);
}

static const struct {
    ZSwipeDraw* draw;
    const FSample* const * sample;
} g_callbacks[Z_SWIPE_NUM] = {
    [Z_SWIPE_FADE_HIDE] = {drawFadeHide, &f_sfx_assets_sfx_swipe_hide_wav},
    [Z_SWIPE_FADE_SHOW] = {drawFadeShow, &f_sfx_assets_sfx_swipe_show_wav},
    [Z_SWIPE_LINES_HIDE] = {drawLinesHide, &f_sfx_assets_sfx_swipe_hide_wav},
    [Z_SWIPE_LINES_SHOW] = {drawLinesShow, &f_sfx_assets_sfx_swipe_show_wav},
};

void z_swipe_start(ZSwipeId Swipe)
{
    g_swipe = Swipe;

    if(g_swipe != Z_SWIPE_INVALID) {
        g_angle = 0;

        f_channel_play(
            F_CHANNEL_ANY, *g_callbacks[g_swipe].sample, F_CHANNEL_PLAY_NORMAL);
    }
}

void z_swipe_stop(void)
{
    g_swipe = Z_SWIPE_INVALID;
}

bool z_swipe_running(void)
{
    return g_swipe != Z_SWIPE_INVALID;
}

void z_swipe_tick(void)
{
    if(g_swipe != Z_SWIPE_INVALID) {
        g_angle += Z_ANGLE_INC;

        if(g_angle > F_DEG_090_FIX) {
            g_swipe = Z_SWIPE_INVALID;
        }
    }
}

void z_swipe_draw(void)
{
    if(g_swipe != Z_SWIPE_INVALID) {
        g_callbacks[g_swipe].draw();
    }
}
