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

#include "obj_map.h"

#include "obj_camera.h"
#include "util_fps.h"
#include "util_graphics.h"

typedef struct {
    uint8_t field[Z_SCREEN_H][Z_SCREEN_W]; // 0 free, 1 captured
} NMap;

static NMap g_map;

void n_map_new(void)
{
    for(int y = 0; y < N_MAP_BORDER_U; y++) {
        for(int x = 0; x < Z_SCREEN_W; x++) {
            g_map.field[y][x] = 1;
        }
    }

    for(int y = Z_SCREEN_H - N_MAP_BORDER_D; y < Z_SCREEN_H; y++) {
        for(int x = 0; x < Z_SCREEN_W; x++) {
            g_map.field[y][x] = 1;
        }
    }

    for(int y = N_MAP_BORDER_U; y < Z_SCREEN_H - N_MAP_BORDER_D; y++) {
        for(int x = 0; x < N_MAP_BORDER_L; x++) {
            g_map.field[y][x] = 1;
        }

        for(int x = Z_SCREEN_W - N_MAP_BORDER_R; x < Z_SCREEN_W; x++) {
            g_map.field[y][x] = 1;
        }
    }
}

void n_map_tick(void)
{
    //
}

void n_map_draw(void)
{
    #define Z_SPEED (8)
    #define Z_DISTORT (4)
    #define Z_INC_MAX (Z_DEG_001_INT * 16)

    #if Z_DISTORT > 16
        #error Z_DISTORT > 16
    #endif

    ZPixel* screen = z_screen_pixelsGet();
    const ZPixel* sprite = z_sprite_pixelsGet(Z_SPRITE_SPACE1, 0);
    ZFixu angleStart = z_fixu_fromInt((z_fps_ticksGet() & (Z_FIX_ANGLES_NUM / Z_SPEED - 1)) * Z_SPEED);

    for(int y = 0; y < Z_SCREEN_H; y++) {
        ZFixu angle = angleStart;
        ZFixu angleInc = 0;

        for(int x = 0; x < Z_SCREEN_W; x++) {
            if((x & 7) == 0) {
                ZFix sinv = z_fix_sin(Z_DEG_180_INT * (unsigned)x / Z_SCREEN_W);
                ZFix mulv = z_fix_mul(sinv, sinv);

                angleInc = (ZFixu)((Z_FIX_ONE - mulv) * Z_INC_MAX);
            }

            angle += angleInc;

            *screen = *(sprite
                        + (y + Z_DISTORT + z_fix_toInt(Z_DISTORT * z_fix_sinf(angle))) * 88
                        + (x + Z_DISTORT + z_fix_toInt(Z_DISTORT * z_fix_cosf(angle))));

            if(g_map.field[y][x] == 1) {
                *screen |= 0xf800;
            }

            screen++;
        }

        ZFix sinv = z_fix_sin(Z_DEG_180_INT * (unsigned)y / Z_SCREEN_H);
        ZFix mulv = z_fix_mul(sinv, sinv);

        angleStart += (ZFixu)((Z_FIX_ONE - mulv) * Z_INC_MAX);
    }
}

bool n_map_wallGet(ZVectorInt Coords)
{
    return g_map.field[Coords.y][Coords.x] > 0;
}

bool n_map_wallGet2(int X, int Y)
{
    return g_map.field[Y][X] > 0;
}

void n_map_wallFill(int X, int Y, int W, int H)
{
    for(int y = H; y--; ) {
        for(int x = W; x--; ) {
            g_map.field[Y + y][X + x] = 1;
        }
    }
}

void n_map_wallBoundsGet(ZVectorInt Origin, int IncX, int IncY, ZVectorInt* Start, ZVectorInt* Dim)
{
    ZVectorInt start = Origin;
    ZVectorInt end = Origin;

    if(IncX == 0) {
        while(!n_map_wallGet(start)) {
            start.x--;
        }

        while(!n_map_wallGet(end)) {
            end.x++;
        }
    } else if(IncX == -1) {
        while(!n_map_wallGet(start)) {
            start.x--;
        }
    } else {
        while(!n_map_wallGet(end)) {
            end.x++;
        }
    }

    Start->x = start.x + (IncX == 1 ? 0 : 1);
    Dim->x = end.x - start.x - (IncX == 0 ? 1 : 0);

    start = Origin;
    end = Origin;

    if(IncY == 0) {
        while(!n_map_wallGet(start)) {
            start.y--;
        }

        while(!n_map_wallGet(end)) {
            end.y++;
        }
    } else if(IncY == -1) {
        while(!n_map_wallGet(start)) {
            start.y--;
        }
    } else {
        while(!n_map_wallGet(end)) {
            end.y++;
        }
    }

    Start->y = start.y + (IncY == 1 ? 0 : 1);
    Dim->y = end.y - start.y - (IncY == 0 ? 1 : 0);
}
