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

#include <faur.h>

#include "util_graphics.h"

extern void z_font_align(FSpriteAlign Alignment);
extern void z_font_printText(int X, int Y, const char* Text);
extern void z_font_printInt(int X, int Y, int Integer);
extern void z_font_printIntu(int X, int Y, unsigned Integer);
extern void z_font_printIntup(int X, int Y, unsigned Integer, int NumDigits);
