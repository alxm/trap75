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

#include "generated/data_gfx_palette.h"

#if Z_PLATFORM_META
typedef struct {
    Image image;
    const uint16_t* buffer;
} ZSprite;

typedef struct {
    const uint8_t* buffer;
    uint32_t size;
} ZSfx;

ZColor z_colors[Z_COLOR_NUM];
static ZSprite g_sprites[Z_SPRITE_NUM];
static ZSfx g_sfx[Z_SFX_NUM];

void z_platform_init(void)
{
}

void z_platform_uninit(void)
{
}

ZPixel* z_screen_pixelsGet(void)
{
    return gb.display._buffer;
}

static void prepLights(ZColorId BgColorId, ZColorId ColorId, int Alpha)
{
    ZPixel color = 0;

    if(ColorId != Z_COLOR_INVALID) {
        int r2 = z_colors[ColorId].rgb.r;
        int g2 = z_colors[ColorId].rgb.g;
        int b2 = z_colors[ColorId].rgb.b;

        if(BgColorId != Z_COLOR_INVALID) {
            int r1 = z_colors[BgColorId].rgb.r;
            int g1 = z_colors[BgColorId].rgb.g;
            int b1 = z_colors[BgColorId].rgb.b;

            color = z_pixel_fromRgb(r1 + (((r2 - r1) * Alpha) >> 8),
                                    g1 + (((g2 - g1) * Alpha) >> 8),
                                    b1 + (((b2 - b1) * Alpha) >> 8));
        } else {
            color = z_pixel_fromRgb((r2 * Alpha) >> 8,
                                    (g2 * Alpha) >> 8,
                                    (b2 * Alpha) >> 8);
        }
    } else if(BgColorId != Z_COLOR_INVALID) {
        color = z_colors[BgColorId].pixel;
    }

    gb.lights.setColor((Color)color);
}

void z_platform_meta_lightsFill(int BgColorId, int ColorId, int Alpha)
{
    prepLights((ZColorId)BgColorId, (ZColorId)ColorId, Alpha);
    gb.lights.fill();
}

void z_platform_meta_lightsDraw(int ColorId, int Alpha, int X, int Y)
{
    prepLights(Z_COLOR_INVALID, (ZColorId)ColorId, Alpha);
    gb.lights.drawPixel(X, Y);
}

void z_platform__loadSprite(int Sprite, const uint16_t* Buffer)
{
    g_sprites[Sprite].image.init(Buffer);
    g_sprites[Sprite].buffer = Buffer;
}

ZPixel z_sprite_transparentColorGet(void)
{
    return g_sprites[Z_SPRITE_PALETTE].buffer[4];
}

const ZPixel* z_sprite_pixelsGet(ZSpriteId Sprite, unsigned Frame)
{
    unsigned dim = g_sprites[Sprite].buffer[0] * g_sprites[Sprite].buffer[1];

    return g_sprites[Sprite].buffer + Z_META_IMAGE_HEADER_LEN + dim * Frame;
}

ZPixel z_sprite_pixelGet(ZSpriteId Sprite, unsigned Frame, int X, int Y)
{
    unsigned w = g_sprites[Sprite].buffer[0];
    unsigned dim = w * g_sprites[Sprite].buffer[1];

    return *(g_sprites[Sprite].buffer
                + Z_META_IMAGE_HEADER_LEN + dim * Frame + Y * w + X);
}

void z_platform_spriteBlit(int Sprite, int X, int Y, unsigned Frame)
{
    g_sprites[Sprite].image.setFrame(Frame);
    gb.display.drawImage(X, Y, g_sprites[Sprite].image);
}

ZVectorInt z_sprite_sizeGet(ZSpriteId Sprite)
{
    return (ZVectorInt){g_sprites[Sprite].buffer[0],
                        g_sprites[Sprite].buffer[1]};
}

int z_sprite_sizeGetWidth(ZSpriteId Sprite)
{
    return g_sprites[Sprite].buffer[0];
}

int z_sprite_sizeGetHeight(ZSpriteId Sprite)
{
    return g_sprites[Sprite].buffer[1];
}

unsigned z_sprite_framesNumGet(ZSpriteId Sprite)
{
    return g_sprites[Sprite].buffer[2];
}

void z_platform__loadSfx(int Sfx, const uint8_t* Buffer, uint32_t Size)
{
    g_sfx[Sfx].buffer = Buffer;
    g_sfx[Sfx].size = Size;
}

void z_sfx_play(ZSfxId Sfx)
{
    gb.sound.stop(0);
    gb.sound.play(g_sfx[Sfx].buffer, g_sfx[Sfx].size);
}

bool z_file_readOnce(const char* FileName, void* Buffer, size_t Size)
{
    File f = SD.open(FileName, O_RDONLY);

    if(!f) {
        return false;
    }

    int ret = f.read(Buffer, Size);

    f.close();

    if(ret < (int)Size) {
        SerialUSB.printf("Read(%s, %u) failed: %d\n", FileName, Size, ret);
        return false;
    }

    return true;
}

bool z_file_writeOnce(const char* FileName, const void* Buffer, size_t Size)
{
    File f = SD.open(FileName, O_RDWR | O_CREAT);

    if(!f) {
        SerialUSB.printf("Cannot open %s for write\n", FileName);
        return false;
    }

    int ret = f.write(Buffer, Size);
    f.close();

    if(ret < (int)Size) {
        SerialUSB.printf("Write(%s, %u) failed: %d\n", FileName, Size, ret);
        return false;
    }

    return true;
}

void z_out(const char* Text)
{
    SerialUSB.printf("%s\n", Text);
}
#endif // Z_PLATFORM_META
