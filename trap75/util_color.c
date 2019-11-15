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

#include "util_color.h"

#include "data_assets.h"

UColor u_colors[U_COLOR_NUM];

void u_color_init(void)
{
    FVectorInt palSize = f_sprite_sizeGet(f_gfx_assets_gfx_palette_png);
    const FColorPixel* pixels =
        f_sprite_pixelsGetBuffer(f_gfx_assets_gfx_palette_png, 0) + palSize.x;

    int color = 0;

    for(int p = palSize.x * palSize.y; p--; ) {
        FColorPixel pixel = *pixels++;

        if(pixel == 0) {
            continue;
        }

        u_colors[color].pixel = pixel;
        u_colors[color].rgb = f_color_pixelToRgb(pixel);

        if(++color == U_COLOR_NUM) {
            break;
        }
    }
}
