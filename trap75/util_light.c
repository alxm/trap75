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

#include "util_light.h"

static const struct {
    UColorId color;
    FFixu counterSpeed[2];
} g_patterns[U_LIGHT_NUM] = {
    [U_LIGHT_GAME_START] = {
        U_COLOR_BG_PURPLE_2,
        {F_DEG_090_FIX / 2, F_DEG_090_FIX / 4},
    },
};

static struct {
    UColorId bgColor;
    ZLightId pulseId;
    FFixu counter;
} g_light = {
    U_COLOR_INVALID,
    U_LIGHT_INVALID,
    0,
};

static struct {
    UColorId bgColor;
    UColorId pulseColor;
    int alpha;
} g_last = {
    U_COLOR_INVALID,
    U_COLOR_INVALID,
    0,
};

void u_light_tick(void)
{
    if(g_light.pulseId != U_LIGHT_INVALID) {
        bool goingDown = g_light.counter >= F_DEG_090_FIX;
        g_light.counter += g_patterns[g_light.pulseId].counterSpeed[goingDown];

        if(g_light.counter >= F_DEG_180_FIX) {
            g_light.pulseId = U_LIGHT_INVALID;
        }
    }
}

void u_light_draw(void)
{
    UColorId color = U_COLOR_INVALID;
    int alpha = 0;

    if(g_light.pulseId != U_LIGHT_INVALID) {
        color = g_patterns[g_light.pulseId].color;
        alpha = f_fix_toInt(f_fix_sinf(g_light.counter) * F_COLOR_ALPHA_MAX);
    }

    if(g_light.bgColor != g_last.bgColor
        || color != g_last.pulseColor || alpha != g_last.alpha) {

        g_last.bgColor = g_light.bgColor;
        g_last.pulseColor = color;
        g_last.alpha = alpha;

        #if F_CONFIG_SYSTEM_GAMEBUINO
            f_gamebuino_lightsStart();

            f_color_blendSet(F_COLOR_BLEND_PLAIN);

            if(g_light.bgColor == U_COLOR_INVALID) {
                f_color_colorSetPixel(0);
            } else {
                f_color_colorSetIndex(g_light.bgColor);
            }

            f_draw_fill();

            if(color != U_COLOR_INVALID) {
                f_color_blendSet(F_COLOR_BLEND_ALPHA);
                f_color_colorSetIndex(color);
                f_color_alphaSet(alpha);

                f_draw_fill();
            }

            f_gamebuino_lightsEnd();
        #endif
    }
}

void u_light_pulseSet(ZLightId Light)
{
    if(g_light.pulseId != Light) {
        g_light.pulseId = Light;
        g_light.counter = 0;
    }
}

void u_light_backgroundSet(UColorId Color)
{
    g_light.bgColor = Color;
}
