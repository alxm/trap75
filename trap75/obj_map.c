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

#include "obj_map.h"

#include "data_assets.h"
#include "obj_camera.h"
#include "util_graphics.h"

typedef struct {
    uint8_t field[F_CONFIG_SCREEN_SIZE_HEIGHT][F_CONFIG_SCREEN_SIZE_WIDTH];
    int numCaptured;
} NMap;

static NMap g_map;

void n_map_new(void)
{
    for(int y = F_CONFIG_SCREEN_SIZE_HEIGHT; y--; ) {
        for(int x = F_CONFIG_SCREEN_SIZE_WIDTH; x--; ) {
            g_map.field[y][x] = 0;
        }
    }

    g_map.numCaptured = 0;
}

void n_map_tick(void)
{
    //
}

void n_map_draw(void)
{
    #define Z_SPEED (8)
    #define Z_DISTORT (4)
    #define Z_INC_MAX (F_DEG_001_INT * 16)

    #if Z_DISTORT > 4
        #error Z_DISTORT > 4
    #endif

    FColorPixel* screen = f_screen_pixelsGetBuffer();
    const FColorPixel* sprite =
        f_sprite_pixelsGetBuffer(f_gfx_assets_gfx_space1_png, 0);
    FFixu angleStart =
        f_fixu_fromInt(
            (f_fps_ticksGet() & (F_FIX_ANGLES_NUM / Z_SPEED - 1)) * Z_SPEED);

    for(int y = 0; y < F_CONFIG_SCREEN_SIZE_HEIGHT; y++) {
        FFixu angle = angleStart;
        FFixu angleInc = 0;

        for(int x = 0; x < F_CONFIG_SCREEN_SIZE_WIDTH; x++) {
            if((x & 7) == 0) {
                FFix sinv = f_fix_sin(F_DEG_180_INT
                                * (unsigned)x / F_CONFIG_SCREEN_SIZE_WIDTH);
                FFix mulv = f_fix_mul(sinv, sinv);

                angleInc = (FFixu)((F_FIX_ONE - mulv) * Z_INC_MAX);
            }

            angle += angleInc;

            *screen =
                *(sprite
                    + (y + Z_DISTORT + f_fix_toInt(Z_DISTORT * f_fix_sinf(angle))) * 88
                    + (x + Z_DISTORT + f_fix_toInt(Z_DISTORT * f_fix_cosf(angle))));

            if(g_map.field[y][x] == 1) {
                #if 1
                    *screen |= 0xf800;
                #else
                    *screen &= 0x07ff;
                    *screen |= 0xd800;
                #endif
            }

            screen++;
        }

        FFix sinv = f_fix_sin(F_DEG_180_INT *
                        (unsigned)y / F_CONFIG_SCREEN_SIZE_HEIGHT);
        FFix mulv = f_fix_mul(sinv, sinv);

        angleStart += (FFixu)((F_FIX_ONE - mulv) * Z_INC_MAX);
    }
}

bool n_map_wallGet(FVectorInt Coords)
{
    return Coords.x < 0
        || Coords.x >= F_CONFIG_SCREEN_SIZE_WIDTH
        || Coords.y < 0
        || Coords.y >= F_CONFIG_SCREEN_SIZE_HEIGHT
        || g_map.field[Coords.y][Coords.x] > 0;
}

bool n_map_wallGet2(int X, int Y)
{
    return X < 0
        || X >= F_CONFIG_SCREEN_SIZE_WIDTH
        || Y < 0
        || Y >= F_CONFIG_SCREEN_SIZE_HEIGHT
        || g_map.field[Y][X] > 0;
}

unsigned n_map_wallFill(int X, int Y, int W, int H)
{
    int area = W * H;

    for(int y = H; y--; ) {
        for(int x = W; x--; ) {
            g_map.field[Y + y][X + x] = 1;
        }
    }

    g_map.numCaptured += area;

    return (unsigned)area;
}

int n_map_wallPercentGet(void)
{
    return 100 * g_map.numCaptured
            / (F_CONFIG_SCREEN_SIZE_WIDTH * F_CONFIG_SCREEN_SIZE_HEIGHT);
}

void n_map_wallBoundsGet(FVectorInt Origin, int IncX, int IncY, FVectorInt* Start, FVectorInt* Dim)
{
    FVectorInt start = Origin;
    FVectorInt end = Origin;

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
