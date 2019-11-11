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

#include "obj_hud.h"

#include "obj_camera.h"
#include "obj_game.h"
#include "obj_map.h"
#include "util_font.h"
#include "util_graphics.h"
#include "util_save.h"

#define N_HUD_ALPHA 192

void n_hud_new(void)
{
    //
}

void n_hud_tick(void)
{
    //
}

static void drawBar(int Value, int Total, int X, int Y, int Width, int Height, ZColorId ColorProg, ZColorId ColorBg, ZColorId ColorBorder)
{
    if(Value > Total) {
        Value = Total;
    }

    int progWidth = Width * Value / Total;

    if(Width > 0 && Value > 0 && progWidth == 0) {
        progWidth = 1;
    }

    // Main bar

    z_graphics_colorSetId(ColorProg);
    z_draw_rectangleAlpha(X, Y, progWidth, Height);

    z_graphics_colorSetId(ColorBg);
    z_draw_rectangleAlpha(X + progWidth, Y, Width - progWidth, Height);

    // Glow border

    z_graphics_alphaSet(N_HUD_ALPHA >> 2);
    z_graphics_colorSetId(ColorBorder);

    z_draw_rectangleAlpha(X, Y - 1, Width, 1);
    z_draw_rectangleAlpha(X, Y + Height, Width, 1);
    z_draw_rectangleAlpha(X - 1, Y, 1, Height);
    z_draw_rectangleAlpha(X + Width, Y, 1, Height);

    z_graphics_alphaSet(N_HUD_ALPHA);
}

static void hudDrawLevel(int X, int Y)
{
    z_graphics_colorSetId(Z_COLOR_CURSOR_TRAIL);
    z_sprite_blitAlphaMask(Z_SPRITE_ICON_LEVEL, 0, X, Y);

    X += z_sprite_sizeGetWidth(Z_SPRITE_ICON_LEVEL) + 1;

    z_graphics_colorSetId(Z_COLOR_CURSOR_MAIN);
    z_font_align(Z_ALIGN_X_LEFT | Z_ALIGN_Y_TOP);
    z_font_printIntup(X, Y, n_game_levelGet() + 1, 2);
}

static void hudDrawPercent(int X, int Y)
{
    drawBar(n_map_wallPercentGet(),
            75,
            X,
            Y,
            20,
            5,
            Z_COLOR_CURSOR_TRAIL,
            Z_COLOR_BG_RED_2,
            Z_COLOR_CURSOR_TRAIL);
}

static void hudDrawScore(int X, int Y)
{
    unsigned score = n_game_scoreGet();

    z_graphics_colorSetId(score > z_save_hiscoreGet()
                            ? Z_COLOR_BALL_YELLOW_2 : Z_COLOR_BALL_YELLOW_1);

    z_font_align(Z_ALIGN_X_LEFT | Z_ALIGN_Y_TOP);
    z_font_printIntup(X, Y, score, 5);
}

static void hudDrawLives(int X, int Y)
{
    z_graphics_colorSetId(Z_COLOR_BG_RED_4);
    z_sprite_blitAlphaMask(Z_SPRITE_ICON_HEART, 0, X, Y - 1);

    X += z_sprite_sizeGetWidth(Z_SPRITE_ICON_HEART) + 1;

    z_graphics_colorSetId(Z_COLOR_CURSOR_TRAIL);
    z_font_align(Z_ALIGN_X_LEFT | Z_ALIGN_Y_TOP);
    z_font_printIntu(X, Y, n_game_livesGet());
}

void n_hud_draw(void)
{
    ZVectorInt shake = n_camera_shakeGet();

    z_graphics_alphaSet(N_HUD_ALPHA);
    z_sprite_align(Z_ALIGN_X_LEFT | Z_ALIGN_Y_TOP);

    hudDrawLevel(3 - shake.x, 3 - shake.y);
    hudDrawPercent(22 - shake.x, 3 + shake.y);
    hudDrawScore(Z_SCREEN_W - 35 + shake.x, 3 - shake.y);
    hudDrawLives(Z_SCREEN_W - 14 + shake.x, 3 + shake.y);
}
