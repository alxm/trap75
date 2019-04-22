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

#include "util_font.h"

static ZSpriteId g_sprite = Z_SPRITE_FONT_AA;

void z_font_set(ZSpriteId Sprite)
{
    g_sprite = Sprite;
}

void z_font_printText(int X, int Y, const char* Text)
{
    int charWidth = z_sprite_sizeGetWidth(g_sprite) + 1;

    z_graphics_alphaSet(256);
    z_sprite_align(Z_ALIGN_X_LEFT | Z_ALIGN_Y_TOP);

    for(char ch = *Text; ch != '\0'; ch = *++Text) {
        z_sprite_blitAlphaMask(g_sprite, ((unsigned)ch - 32), X, Y);
        X += charWidth;
    }
}

void z_font_printInt(int X, int Y, int Integer)
{
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d", Integer);

    z_font_printText(X, Y, buffer);
}

void z_font_printIntu(int X, int Y, unsigned Integer)
{
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%u", Integer);

    z_font_printText(X, Y, buffer);
}
