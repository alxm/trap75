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

extern void n_game_new(void);

extern void n_game_tick(void);
extern void n_game_draw(void);

extern unsigned n_game_levelGet(void);
extern void n_game_levelSet(unsigned Level);

extern unsigned n_game_scoreGet(void);
extern void n_game_scoreAdd(unsigned Points);

extern unsigned n_game_livesGet(void);
extern void n_game_livesDec(void);
