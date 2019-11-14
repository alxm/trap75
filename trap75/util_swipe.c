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
    f_color_blendSet(F_COLOR_BLEND_ALPHA);
    f_color_colorSetPixel(z_colors[Z_COLOR_BG_PURPLE_1].pixel);
    f_color_alphaSet(f_fix_toInt(f_fix_sinf(g_angle) * F_COLOR_ALPHA_MAX));

    f_draw_rectangle(
        0, 0, F_CONFIG_SCREEN_SIZE_WIDTH, F_CONFIG_SCREEN_SIZE_HEIGHT);
}

static void drawFadeShow(void)
{
    f_color_blendSet(F_COLOR_BLEND_ALPHA);
    f_color_colorSetPixel(z_colors[Z_COLOR_BG_PURPLE_1].pixel);
    f_color_alphaSet(
        f_fix_toInt(f_fix_sinf(F_DEG_090_FIX - g_angle) * F_COLOR_ALPHA_MAX));

    f_draw_rectangle(
        0, 0, F_CONFIG_SCREEN_SIZE_WIDTH, F_CONFIG_SCREEN_SIZE_HEIGHT);
}

static void drawLines(FFixu Angle)
{
    FFix sine = f_fix_sinf(Angle);
    int h = f_fix_toInt(sine * (F_CONFIG_SCREEN_SIZE_HEIGHT / 2));
    int alpha = f_fix_toInt(sine * F_COLOR_ALPHA_MAX);

    f_color_blendSet(F_COLOR_BLEND_ALPHA);
    f_color_colorSetPixel(z_colors[Z_COLOR_BG_PURPLE_1].pixel);
    f_color_alphaSet(alpha);

    f_draw_rectangle(0, 0, F_CONFIG_SCREEN_SIZE_WIDTH, h);
    f_draw_rectangle(
        0, F_CONFIG_SCREEN_SIZE_HEIGHT - h, F_CONFIG_SCREEN_SIZE_WIDTH, h);

    f_color_blendSet(F_COLOR_BLEND_PLAIN);
    f_color_colorSetPixel(z_colors[Z_COLOR_BG_PURPLE_2].pixel);

    f_draw_hline(0, F_CONFIG_SCREEN_SIZE_WIDTH - 1, h);
    f_draw_hline(
        0, F_CONFIG_SCREEN_SIZE_WIDTH - 1, F_CONFIG_SCREEN_SIZE_HEIGHT - h - 1);
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
