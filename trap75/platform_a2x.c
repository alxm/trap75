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

#include "platform.h"

#include "util_graphics.h"
#include "util_input.h"
#include "util_sound.h"
#include "util_state.h"

#if Z_PLATFORM_FAUR
typedef struct {
    FSample* sample;
    int channel;
} ZSfx;

ZColor z_colors[Z_COLOR_NUM];
static FSprite* g_sprites[Z_SPRITE_NUM];
static ZSfx g_sfx[Z_SFX_NUM];

void z_platform_init(void)
{
}

void z_platform_uninit(void)
{
    for(int s = 0; s < Z_SPRITE_NUM; s++) {
        f_sprite_free(g_sprites[s]);
    }

    for(int s = 0; s < Z_SFX_NUM; s++) {
        f_sample_free(g_sfx[s].sample);
    }
}

ZPixel* z_screen_pixelsGet(void)
{
    return f_screen_pixelsGetBuffer();
}

void z_platform__loadSprite(int Sprite, const char* Path)
{
    g_sprites[Sprite] = f_sprite_newFromPng(Path, 0, 0, 0, 0);
}

ZPixel z_sprite_transparentColorGet(void)
{
    return f_color_pixelFromHex(F_CONFIG_COLOR_SPRITE_KEY);
}

const ZPixel* z_sprite_pixelsGet(ZSpriteId Sprite, unsigned Frame)
{
    return f_sprite_pixelsGetBuffer(g_sprites[Sprite], Frame);
}

ZPixel z_sprite_pixelGet(ZSpriteId Sprite, unsigned Frame, int X, int Y)
{
    return f_sprite_pixelsGetValue(g_sprites[Sprite], Frame, X, Y);
}

void z_platform_spriteBlit(int Sprite, int X, int Y, unsigned Frame)
{
    f_sprite_blit(g_sprites[Sprite], Frame, X, Y);
}

FVectorInt z_sprite_sizeGet(ZSpriteId Sprite)
{
    FVectorInt size = f_sprite_sizeGet(g_sprites[Sprite]);

    return (FVectorInt){size.x, size.y};
}

int z_sprite_sizeGetWidth(ZSpriteId Sprite)
{
    return f_sprite_sizeGetWidth(g_sprites[Sprite]);
}

int z_sprite_sizeGetHeight(ZSpriteId Sprite)
{
    return f_sprite_sizeGetHeight(g_sprites[Sprite]);
}

unsigned z_sprite_framesNumGet(ZSpriteId Sprite)
{
    return f_sprite_framesNumGet(g_sprites[Sprite]);
}

void z_platform__loadSfx(int Sfx, const char* Path)
{
    g_sfx[Sfx].sample = f_sample_new(Path);
    g_sfx[Sfx].channel = f_channel_new();
}

void z_sfx_play(ZSfxId Sfx)
{
    f_channel_play(
        g_sfx[Sfx].channel, g_sfx[Sfx].sample, F_CHANNEL_PLAY_RESTART);
}
#endif // Z_PLATFORM_FAUR
