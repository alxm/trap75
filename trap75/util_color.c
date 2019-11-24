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

FPalette* g_palette;

void u_color_init(void)
{
    g_palette = f_palette_newFromSprite(f_gfx_assets_gfx_palette_png);

    f_color_paletteSet(g_palette);
}

void u_color_uninit(void)
{
    f_palette_free(g_palette);
}
