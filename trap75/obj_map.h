/*
    Copyright 2019-2020 Alex Margarit <alex@alxm.org>
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

#include <faur.h>

extern void n_map_new(void);

extern void n_map_tick(void);
extern void n_map_draw(void);

extern bool n_map_test(FVectorInt Coords);
extern unsigned n_map_fill(int X, int Y, int W, int H);
extern int n_map_percentGet(void);
extern void n_map_boundsGet(FVectorInt Origin, int IncX, int IncY, FVectorInt* Start, FVectorInt* Dim);
