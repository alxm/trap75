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

#include "util_graphics.h"

#include "generated/data_gfx_palette.h"

#include "generated/data_gfx_alxm.h"
#include "generated/data_gfx_alxm_wing.h"

#include "generated/data_gfx_icon_heart.h"
#include "generated/data_gfx_icon_level.h"

#include "generated/data_gfx_space1.h"

#include "generated/data_gfx_cursor.h"

#include "generated/data_gfx_ball1.h"
#include "generated/data_gfx_ball2.h"
#include "generated/data_gfx_ball3.h"
#include "generated/data_gfx_ball4.h"

#include "generated/data_gfx_font_lcdnum_grid4x7.h"
#include "generated/data_gfx_font_smallnum_grid3x5.h"

static ZRgb g_rgb;
static int g_alpha;
static ZAlign g_align;

void z_graphics_setup(void)
{
    z_sprite_load(Z_SPRITE_PALETTE, palette);

    z_sprite_load(Z_SPRITE_ALXM, alxm);
    z_sprite_load(Z_SPRITE_ALXM_WING, alxm_wing);

    z_sprite_load(Z_SPRITE_ICON_HEART, icon_heart);
    z_sprite_load(Z_SPRITE_ICON_LEVEL, icon_level);

    z_sprite_load(Z_SPRITE_SPACE1, space1);

    z_sprite_load(Z_SPRITE_CURSOR, cursor);

    z_sprite_load(Z_SPRITE_BALL1, ball1);
    z_sprite_load(Z_SPRITE_BALL2, ball2);
    z_sprite_load(Z_SPRITE_BALL3, ball3);
    z_sprite_load(Z_SPRITE_BALL4, ball4);

    z_sprite_load(Z_SPRITE_FONT_LCDNUM, font_lcdnum_grid4x7);
    z_sprite_load(Z_SPRITE_FONT_SMALLNUM, font_smallnum_grid3x5);

    int palWidth = z_sprite_sizeGetWidth(Z_SPRITE_PALETTE);
    int palHeight = z_sprite_sizeGetHeight(Z_SPRITE_PALETTE) - 1;
    const ZPixel* pixels = z_sprite_pixelsGet(Z_SPRITE_PALETTE, 0) + palWidth;
    int color = 0;

    for(int p = palWidth * palHeight; p--; ) {
        ZPixel pixel = *pixels++;

        if(pixel == 0) {
            continue;
        }

        z_colors[color].pixel = pixel;
        z_colors[color].rgb = z_pixel_toRgb(pixel);

        if(++color == Z_COLOR_NUM) {
            break;
        }
    }

    g_align = Z_ALIGN_X_LEFT | Z_ALIGN_Y_TOP;
}

void z_graphics_colorSetId(ZColorId Color)
{
    g_rgb = z_colors[Color].rgb;
}

void z_graphics_colorSetRgb(const ZRgb* Rgb)
{
    g_rgb = *Rgb;
}

void z_graphics_alphaSet(int Alpha)
{
    g_alpha = Alpha;
}

void z_sprite_align(ZAlign Alignment)
{
    g_align = Alignment;
}

void z_sprite_blit(ZSpriteId Sprite, unsigned Frame, int X, int Y)
{
    ZVectorInt spriteSize = z_sprite_sizeGet(Sprite);

    if(g_align & Z_ALIGN_X_CENTER) {
        X -= spriteSize.x >> 1;
    } else if(g_align & Z_ALIGN_X_RIGHT) {
        X -= spriteSize.x;
    }

    if(g_align & Z_ALIGN_Y_CENTER) {
        Y -= spriteSize.y >> 1;
    } else if(g_align & Z_ALIGN_Y_BOTTOM) {
        Y -= spriteSize.y;
    }

    z_platform_spriteBlit(Sprite, X, Y, Frame);
}

void z_sprite_blitAlphaMask(ZSpriteId AlphaMask, unsigned Frame, int X, int Y)
{
    if(g_alpha == 0) {
        return;
    }

    ZVectorInt spriteSize = z_sprite_sizeGet(AlphaMask);

    if(g_align & Z_ALIGN_X_CENTER) {
        X -= spriteSize.x >> 1;
    } else if(g_align & Z_ALIGN_X_RIGHT) {
        X -= spriteSize.x;
    }

    if(g_align & Z_ALIGN_Y_CENTER) {
        Y -= spriteSize.y >> 1;
    } else if(g_align & Z_ALIGN_Y_BOTTOM) {
        Y -= spriteSize.y;
    }

    if(X >= Z_SCREEN_W || X + spriteSize.x <= 0
        || Y >= Z_SCREEN_H || Y + spriteSize.y <= 0) {

        return;
    }

    ZPixel* screenPixels = z_screen_pixelsGet();
    const ZPixel* spritePixels = z_sprite_pixelsGet(AlphaMask, Frame);

    int spriteDrawW = spriteSize.x;
    int spriteDrawH = spriteSize.y;

    if(X < 0) {
        spritePixels += -X;
        spriteDrawW -= -X;
        X = 0;
    }

    if(X + spriteDrawW > Z_SCREEN_W) {
        spriteDrawW = Z_SCREEN_W - X;
    }

    if(Y < 0) {
        spritePixels += -Y * spriteSize.x;
        spriteDrawH -= -Y;
        Y = 0;
    }

    if(Y + spriteDrawH > Z_SCREEN_H) {
        spriteDrawH = Z_SCREEN_H - Y;
    }

    screenPixels += Y * Z_SCREEN_W + X;

    for(int y = spriteDrawH; y--; ) {
        ZPixel* screenPixels2 = screenPixels;
        const ZPixel* spritePixels2 = spritePixels;

        for(int x = spriteDrawW; x--; ) {
            int a = (g_alpha * z_pixel_toAnyChannel(*spritePixels2++)) >> 8;

            if(a != 0) {
                z_draw_pixelBufferRgbAlpha(screenPixels2, &g_rgb, a);
            }

            screenPixels2++;
        }

        screenPixels += Z_SCREEN_W;
        spritePixels += spriteSize.x;
    }
}

void z_draw_fill(void)
{
    const ZPixel pixel = z_pixel_fromRgbStruct(&g_rgb);
    ZPixel* pixels = z_screen_pixelsGet();

    for(int i = Z_SCREEN_W * Z_SCREEN_H; i--; ) {
        *pixels++ = pixel;
    }
}

void z_draw_rectangle(int X, int Y, int W, int H)
{
    if(X >= Z_SCREEN_W || X + W <= 0 || Y >= Z_SCREEN_H || Y + H <= 0) {
        return;
    }

    if(X < 0) {
        W -= -X;
        X = 0;
    }

    if(X + W > Z_SCREEN_W) {
        W = Z_SCREEN_W - X;
    }

    if(Y < 0) {
        H -= -Y;
        Y = 0;
    }

    if(Y + H > Z_SCREEN_H) {
        H = Z_SCREEN_H - Y;
    }

    const ZPixel pixel = z_pixel_fromRgbStruct(&g_rgb);
    ZPixel* screenPixels = z_screen_pixelsGet() + Y * Z_SCREEN_W + X;

    for(int y = H; y--; ) {
        ZPixel* screenPixels2 = screenPixels;

        for(int x = W; x--; ) {
            *screenPixels2++ = pixel;
        }

        screenPixels += Z_SCREEN_W;
    }
}

void z_draw_rectangleAlpha(int X, int Y, int W, int H)
{
    if(g_alpha == 0
        || X >= Z_SCREEN_W || X + W <= 0 || Y >= Z_SCREEN_H || Y + H <= 0) {

        return;
    }

    if(X < 0) {
        W -= -X;
        X = 0;
    }

    if(X + W > Z_SCREEN_W) {
        W = Z_SCREEN_W - X;
    }

    if(Y < 0) {
        H -= -Y;
        Y = 0;
    }

    if(Y + H > Z_SCREEN_H) {
        H = Z_SCREEN_H - Y;
    }

    ZPixel* screenPixels = z_screen_pixelsGet() + Y * Z_SCREEN_W + X;

    for(int y = H; y--; ) {
        ZPixel* screenPixels2 = screenPixels;

        for(int x = W; x--; ) {
            z_draw_pixelBufferRgbAlpha(screenPixels2++, &g_rgb, g_alpha);
        }

        screenPixels += Z_SCREEN_W;
    }
}

void z_draw_pixelAlpha(int X, int Y)
{
    if(g_alpha == 0 || X < 0 || X >= Z_SCREEN_W || Y < 0 || Y >= Z_SCREEN_H) {
        return;
    }

    z_draw_pixelCoordsRgbAlpha(X, Y, &g_rgb, g_alpha);
}
