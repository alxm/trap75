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

#define O_BALL_NUM_MAX 16

typedef enum {
    O_BALL_ID_INVALID = -1,
    O_BALL_ID_1,
    O_BALL_ID_2,
    O_BALL_ID_3,
    O_BALL_ID_4,
    O_BALL_ID_NUM
} OBallId;

typedef struct OBall OBall;

extern void o_ball_setup(void);

extern void o_ball_new(OBallId Id, int X, int Y, unsigned Angle);

extern void o_ball_tick(void);
extern void o_ball_draw(void);

extern bool o_ball_checkArea(FVectorInt Start, FVectorInt Dim);
extern bool o_ball_checkArea2(FFix X, FFix Y, FFix W, FFix H);
