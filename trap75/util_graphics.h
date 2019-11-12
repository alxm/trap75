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

#pragma once

#include "platform.h"

Z_EXTERN_C_START

#define Z_SCREEN_W F_CONFIG_SCREEN_SIZE_WIDTH
#define Z_SCREEN_H F_CONFIG_SCREEN_SIZE_HEIGHT

typedef enum {
    Z_COLOR_INVALID = -1,

    Z_COLOR_ALXM_BG,
    Z_COLOR_ALXM_Y1,
    Z_COLOR_ALXM_Y2,
    Z_COLOR_ALXM_G1,
    Z_COLOR_ALXM_G2,
    Z_COLOR_ALXM_B1,
    Z_COLOR_ALXM_B2,
    Z_COLOR_ALXM_R1,
    Z_COLOR_ALXM_R2,

    Z_COLOR_BG_PURPLE_1,
    Z_COLOR_BG_PURPLE_2,
    Z_COLOR_BG_GREEN_1,
    Z_COLOR_BG_GREEN_2,
    Z_COLOR_BG_RED_1,
    Z_COLOR_BG_RED_2,
    Z_COLOR_BG_RED_3,
    Z_COLOR_BG_RED_4,

    Z_COLOR_BALL_YELLOW_1,
    Z_COLOR_BALL_YELLOW_2,
    Z_COLOR_BALL_YELLOW_3,
    Z_COLOR_BALL_YELLOW_4,

    Z_COLOR_CURSOR_TRAIL,
    Z_COLOR_CURSOR_MAIN,

    Z_COLOR_NUM
} ZColorId;

typedef enum {
    Z_SPRITE_INVALID = -1,
    Z_SPRITE_PALETTE,

    Z_SPRITE_ALXM,
    Z_SPRITE_ALXM_WING,
    Z_SPRITE_ALXM_FOOTER,

    Z_SPRITE_TITLE,
    Z_SPRITE_TITLE_GLOW,

    Z_SPRITE_ICON_HEART,
    Z_SPRITE_ICON_LEVEL,

    Z_SPRITE_SPACE1,

    Z_SPRITE_CURSOR,

    Z_SPRITE_BALL1,
    Z_SPRITE_BALL2,
    Z_SPRITE_BALL3,
    Z_SPRITE_BALL4,

    Z_SPRITE_FONT_AA,

    Z_SPRITE_NUM
} ZSpriteId;

typedef enum {
    Z_ALIGN_X_LEFT = Z_FLAG_BIT(1),
    Z_ALIGN_X_CENTER = Z_FLAG_BIT(2),
    Z_ALIGN_X_RIGHT = Z_FLAG_BIT(3),
    Z_ALIGN_Y_TOP = Z_FLAG_BIT(4),
    Z_ALIGN_Y_CENTER = Z_FLAG_BIT(5),
    Z_ALIGN_Y_BOTTOM = Z_FLAG_BIT(6),
} ZAlign;

typedef struct ZColor {
    ZPixel pixel;
    ZRgb rgb;
} ZColor;

extern ZColor z_colors[Z_COLOR_NUM];

extern void z_graphics_setup(void);

extern void z_graphics_colorSetId(ZColorId Color);
extern void z_graphics_colorSetRgb(const ZRgb* Rgb);
extern void z_graphics_alphaSet(int Alpha);

extern ZPixel* z_screen_pixelsGet(void);

extern void z_sprite_align(ZAlign Alignment);
extern ZPixel z_sprite_transparentColorGet(void);
extern const ZPixel* z_sprite_pixelsGet(ZSpriteId Sprite, unsigned Frame);
extern ZPixel z_sprite_pixelGet(ZSpriteId Sprite, unsigned Frame, int X, int Y);
extern void z_sprite_blit(ZSpriteId Sprite, unsigned Frame, int X, int Y);
extern void z_sprite_blitAlphaMask(ZSpriteId AlphaMask, unsigned Frame, int X, int Y);
extern FVectorInt z_sprite_sizeGet(ZSpriteId Sprite);
extern int z_sprite_sizeGetWidth(ZSpriteId Sprite);
extern int z_sprite_sizeGetHeight(ZSpriteId Sprite);
extern unsigned z_sprite_framesNumGet(ZSpriteId Sprite);

static inline ZPixel z_pixel_fromRgbStruct(const ZRgb* Rgb)
{
    return z_pixel_fromRgb(Rgb->r, Rgb->g, Rgb->b);
}

extern void z_draw_fill(void);
extern void z_draw_rectangle(int X, int Y, int W, int H);
extern void z_draw_rectangleAlpha(int X, int Y, int W, int H);
extern void z_draw_pixelAlpha(int X, int Y);

static inline void z_draw_hline(int X1, int X2, int Y)
{
    z_draw_rectangle(X1, Y, X2 - X1 + 1, 1);
}

static inline void z_draw_pixelBufferRgbAlpha(ZPixel* Dst, const ZRgb* Rgb, int Alpha)
{
    ZRgb rgb = z_pixel_toRgb(*Dst);

    *Dst = z_pixel_fromRgb(rgb.r + (((Rgb->r - rgb.r) * Alpha) >> 8),
                           rgb.g + (((Rgb->g - rgb.g) * Alpha) >> 8),
                           rgb.b + (((Rgb->b - rgb.b) * Alpha) >> 8));
}

static inline void z_draw_pixelCoordsIdAlpha(int X, int Y, ZColorId Color, int Alpha)
{
    z_draw_pixelBufferRgbAlpha(
        z_screen_pixelsGet() + Y * Z_SCREEN_W + X, &z_colors[Color].rgb, Alpha);
}

static inline void z_draw_pixelCoordsRgbAlpha(int X, int Y, const ZRgb* Rgb, int Alpha)
{
    z_draw_pixelBufferRgbAlpha(
        z_screen_pixelsGet() + Y * Z_SCREEN_W + X, Rgb, Alpha);
}

Z_EXTERN_C_END
