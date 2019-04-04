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

#include "obj_game.h"

#include "obj_ball.h"
#include "obj_camera.h"
#include "obj_cursor.h"
#include "obj_map.h"
#include "obj_hud.h"
#include "util_save.h"

typedef struct {
    unsigned level;
    unsigned score;
} NGame;

static NGame g_game;

void n_game_new(unsigned Level)
{
    n_map_new();
    n_camera_new();
    n_hud_new();

    g_game.level = Level;
    g_game.score = 0;

    n_cursor_new();
    o_ball_setup();
}

void n_game_tick(void)
{
    n_camera_tick();
    n_map_tick();
    o_ball_tick();
    n_cursor_tick();
    n_hud_tick();
}

void n_game_draw(void)
{
    n_map_draw();
    o_ball_draw();
    n_cursor_draw();
    n_hud_draw();
}

unsigned n_game_levelGet(void)
{
    return g_game.level;
}

unsigned n_game_scoreGet(void)
{
    return g_game.score;
}

void n_game_scoreAdd(unsigned Points)
{
    g_game.score += Points;
}
