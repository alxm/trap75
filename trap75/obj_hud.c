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
#include "util_graphics.h"

void n_hud_new(void)
{
    //
}

void n_hud_tick(void)
{
    //
}

static void drawNumber(int X, int Y, unsigned Number, int NumDigits, ZSpriteId Font)
{
    int charSize = z_sprite_sizeGetWidth(Font) + 1;

    X += charSize * (NumDigits - 1);

    z_graphics_alphaSet(256);
    z_sprite_align(Z_ALIGN_X_LEFT | Z_ALIGN_Y_TOP);

    for(int d = NumDigits; d--; X -= charSize, Number /= 10) {
        z_sprite_blitAlphaMask(Font, Number % 10, X, Y);
    }
}

static void hudDrawScore(int X, int Y)
{
    z_graphics_colorSetId(Z_COLOR_CURSOR_TRAIL);

    drawNumber(X, Y, n_game_scoreGet(), 4, Z_SPRITE_FONT_LCDNUM);
}

static void hudDrawLevel(int X, int Y)
{
    z_graphics_colorSetId(Z_COLOR_CURSOR);
    z_graphics_alphaSet(192);

    drawNumber(X, Y, n_game_levelGet() + 1, 2, Z_SPRITE_FONT_LCDNUM);
}

static void hudDrawPercent(int X, int Y)
{
    z_graphics_colorSetId(Z_COLOR_CURSOR_TRAIL);

    drawNumber(X, Y, (unsigned)n_map_wallPercentGet(), 2, Z_SPRITE_FONT_LCDNUM);
}

void n_hud_draw(void)
{
    ZVectorInt shake = n_camera_shakeGet();

    hudDrawScore(Z_SCREEN_W - 20 - shake.x, 1 + shake.y);
    hudDrawLevel(1 - shake.x, 1 - shake.y);
    hudDrawPercent(Z_SCREEN_W / 4 + shake.x, 1 + shake.y);
}
