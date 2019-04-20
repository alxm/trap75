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

#pragma once

#include "platform.h"

#include "util_fix.h"

extern void n_map_new(void);

extern void n_map_tick(void);
extern void n_map_draw(void);

extern bool n_map_wallGet(ZVectorInt Coords);
extern bool n_map_wallGet2(int X, int Y);
extern void n_map_wallFill(int X, int Y, int W, int H);
extern int n_map_wallPercentGet(void);
extern void n_map_wallBoundsGet(ZVectorInt origin, int IncX, int IncY, ZVectorInt* Start, ZVectorInt* Dim);
