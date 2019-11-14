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

#include "obj_hud.h"

#include "data_assets.h"
#include "obj_camera.h"
#include "obj_game.h"
#include "obj_map.h"
#include "util_graphics.h"
#include "util_save.h"

#define N_HUD_ALPHA 192

void n_hud_new(void)
{
    //
}

void n_hud_tick(void)
{
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

    f_color_colorSetPixel(z_colors[ColorProg].pixel);
    f_draw_rectangle(X, Y, progWidth, Height);

    f_color_colorSetPixel(z_colors[ColorBg].pixel);
    f_draw_rectangle(X + progWidth, Y, Width - progWidth, Height);

    // Glow border

    f_color_colorSetPixel(z_colors[ColorBorder].pixel);
    f_color_alphaSet(N_HUD_ALPHA >> 2);

    f_draw_rectangle(X, Y - 1, Width, 1);
    f_draw_rectangle(X, Y + Height, Width, 1);
    f_draw_rectangle(X - 1, Y, 1, Height);
    f_draw_rectangle(X + Width, Y, 1, Height);

    f_color_alphaSet(N_HUD_ALPHA);
}

static void hudDrawLevel(int X, int Y)
{
    f_color_colorSetPixel(z_colors[Z_COLOR_CURSOR_TRAIL].pixel);
    f_sprite_blit(f_gfx_assets_gfx_icon_level_png, 0, X, Y);

    X += f_sprite_sizeGetWidth(f_gfx_assets_gfx_icon_level_png) + 1;

    f_color_colorSetPixel(z_colors[Z_COLOR_CURSOR_MAIN].pixel);
    f_font_coordsSet(X, Y);
    f_font_printf("%0*u", 2, n_game_levelGet());
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

    ZColorId color = score > z_save_hiscoreGet()
                        ? Z_COLOR_BALL_YELLOW_2 : Z_COLOR_BALL_YELLOW_1;
    f_color_colorSetPixel(z_colors[color].pixel);

    f_font_coordsSet(X, Y);
    f_font_printf("%0*u", 5, score);
}

static void hudDrawLives(int X, int Y)
{
    f_color_colorSetPixel(z_colors[Z_COLOR_BG_RED_4].pixel);
    f_sprite_blit(f_gfx_assets_gfx_icon_heart_png, 0, X, Y - 1);

    X += f_sprite_sizeGetWidth(f_gfx_assets_gfx_icon_heart_png) + 1;

    f_color_colorSetPixel(z_colors[Z_COLOR_CURSOR_TRAIL].pixel);
    f_font_coordsSet(X, Y);
    f_font_printf("%u", n_game_livesGet());
}

void n_hud_draw(void)
{
    FVectorInt shake = n_camera_shakeGet();

    f_color_blendSet(F_COLOR_BLEND_ALPHA_MASK);
    f_color_alphaSet(N_HUD_ALPHA);
    f_sprite_alignSet(F_SPRITE_ALIGN_X_LEFT | F_SPRITE_ALIGN_Y_TOP);

    f_font_alignSet(F_FONT_ALIGN_LEFT);

    hudDrawLevel(3 - shake.x, 3 - shake.y);
    hudDrawPercent(22 - shake.x, 3 + shake.y);
    hudDrawScore(F_CONFIG_SCREEN_SIZE_WIDTH - 35 + shake.x, 3 - shake.y);
    hudDrawLives(F_CONFIG_SCREEN_SIZE_WIDTH - 14 + shake.x, 3 + shake.y);
}
