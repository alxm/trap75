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

#include "obj_game.h"

#include "obj_ball.h"
#include "obj_camera.h"
#include "obj_cursor.h"
#include "obj_map.h"
#include "obj_hud.h"
#include "util_graphics.h"
#include "util_save.h"

#define N_GAME_LEVELS_NUM 8
#define N_GAME_LIVES_START 2

typedef struct {
    unsigned level;
    unsigned score;
    unsigned lives;
} NGame;

static NGame g_game;

void n_game_new(void)
{
    g_game.score = 0;
    g_game.lives = N_GAME_LIVES_START;

    n_game_levelSet(0);
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

void n_game_levelSet(unsigned Level)
{
    g_game.level = Level;

    n_map_new();
    n_camera_new();
    n_hud_new();
    n_cursor_new();
    o_ball_setup();

    if(Level >= N_GAME_LEVELS_NUM) {
        return;
    }

    static const OBallId levels[N_GAME_LEVELS_NUM][O_BALL_NUM_MAX + 1] = {
        {O_BALL_ID_1, O_BALL_ID_1, O_BALL_ID_INVALID},
        {O_BALL_ID_1, O_BALL_ID_1, O_BALL_ID_2, O_BALL_ID_INVALID},
        {O_BALL_ID_1, O_BALL_ID_2, O_BALL_ID_1, O_BALL_ID_2, O_BALL_ID_INVALID},
        {O_BALL_ID_1, O_BALL_ID_2, O_BALL_ID_1, O_BALL_ID_2, O_BALL_ID_3, O_BALL_ID_INVALID},
        {O_BALL_ID_1, O_BALL_ID_3, O_BALL_ID_1, O_BALL_ID_3, O_BALL_ID_1, O_BALL_ID_3, O_BALL_ID_INVALID},
        {O_BALL_ID_2, O_BALL_ID_4, O_BALL_ID_2, O_BALL_ID_3, O_BALL_ID_3, O_BALL_ID_INVALID},
        {O_BALL_ID_1, O_BALL_ID_2, O_BALL_ID_4, O_BALL_ID_1, O_BALL_ID_2, O_BALL_ID_4, O_BALL_ID_INVALID},
        {O_BALL_ID_3, O_BALL_ID_1, O_BALL_ID_4, O_BALL_ID_1, O_BALL_ID_4, O_BALL_ID_1, O_BALL_ID_3, O_BALL_ID_1, O_BALL_ID_INVALID},
    };

    unsigned numBalls = 0;

    while(levels[Level][numBalls] != O_BALL_ID_INVALID) {
        numBalls++;
    }

    unsigned angleInc = F_FIX_ANGLES_NUM / numBalls;
    unsigned angle = angleInc * 2 / 3;

    for(unsigned b = 0; b < numBalls; b++) {
        o_ball_new(levels[Level][b],
                   F_CONFIG_SCREEN_SIZE_WIDTH / 2
                    + f_fix_toInt(f_fix_cos(angle) * 16),
                   F_CONFIG_SCREEN_SIZE_HEIGHT / 2
                    - f_fix_toInt(f_fix_sin(angle) * 16),
                   angle);

        angle += angleInc;
    }
}

unsigned n_game_scoreGet(void)
{
    return g_game.score;
}

void n_game_scoreAdd(unsigned Points)
{
    g_game.score += Points;
}

unsigned n_game_livesGet(void)
{
    return g_game.lives;
}

void n_game_livesDec(void)
{
    if(g_game.lives > 0) {
        g_game.lives--;
    }
}
