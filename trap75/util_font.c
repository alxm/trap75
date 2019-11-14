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

#include "util_font.h"

#include "data_assets.h"

static const FSprite *const *const g_sprite = &f_gfx_assets_gfx_font_aa_3x5_png;
static FSpriteAlign g_align = F_SPRITE_ALIGN_X_LEFT | F_SPRITE_ALIGN_Y_TOP;

void z_font_align(FSpriteAlign Alignment)
{
    g_align = Alignment;
}

void z_font_printText(int X, int Y, const char* Text)
{
    const FSprite* sprite = *g_sprite;
    int charWidth = f_sprite_sizeGetWidth(sprite) + 1;

    if(g_align & F_SPRITE_ALIGN_Y_CENTER) {
        Y -= f_sprite_sizeGetHeight(sprite) / 2;
    } else if(g_align & F_SPRITE_ALIGN_Y_BOTTOM) {
        Y -= f_sprite_sizeGetHeight(sprite);
    }

    if(g_align & (F_SPRITE_ALIGN_X_CENTER | F_SPRITE_ALIGN_X_RIGHT)) {
        int tally = -1;

        for(const char* text = Text; *text++ != '\0'; ) {
            tally += charWidth;
        }

        if(g_align & F_SPRITE_ALIGN_X_CENTER) {
            X -= tally / 2;
        } else {
            X -= tally;
        }
    }

    f_sprite_alignSet(F_SPRITE_ALIGN_X_LEFT | F_SPRITE_ALIGN_Y_TOP);

    for(char ch = *Text; ch != '\0'; ch = *++Text) {
        f_sprite_blit(sprite, ((unsigned)ch - 32), X, Y);
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

void z_font_printIntup(int X, int Y, unsigned Integer, int NumDigits)
{
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%0*u", NumDigits, Integer);

    z_font_printText(X, Y, buffer);
}
