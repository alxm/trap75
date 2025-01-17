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

#include "obj_ball.h"

#include "obj_cursor.h"
#include "obj_map.h"
#include "util_color.h"

#define O_BALL_TRAIL_ALPHA 64
#define O_BALL_HISTORY_LEN 8

typedef struct {
    const FSprite* const * sprite;
    int radius;
} OBallData;

static const OBallData g_ballsData[O_BALL_ID_NUM] = {
    [O_BALL_ID_1] = {&FSprite_embed_gfx_ball1_png, 1},
    [O_BALL_ID_2] = {&FSprite_embed_gfx_ball2_png, 2},
    [O_BALL_ID_3] = {&FSprite_embed_gfx_ball3_png, 3},
    [O_BALL_ID_4] = {&FSprite_embed_gfx_ball4_png, 5},
};

struct OBall {
    OBallId id;
    FVecFix coords, coordsNext;
    FVecInt coordsHistory[O_BALL_HISTORY_LEN];
    FVecFix velocity, velocityNext;
    bool hitWall, hitBall, committed, ignore;
};

static OBall g_balls[O_BALL_NUM_MAX];
static int g_tail;

void o_ball_setup(void)
{
    g_tail = 0;
}

void o_ball_new(OBallId Id, int X, int Y, unsigned Angle)
{
    if(g_tail == O_BALL_NUM_MAX) {
        return;
    }

    OBall* b = &g_balls[g_tail++];

    b->id = Id;

    b->coords.x = f_fix_fromInt(X);
    b->coords.y = f_fix_fromInt(Y);

    b->coordsHistory[0] = f_vecfix_toInt(b->coords);

    for(int i = 1; i < O_BALL_HISTORY_LEN; i++) {
        b->coordsHistory[i] = b->coordsHistory[0];
    }

    b->velocity.x = +f_fix_cos(Angle);
    b->velocity.y = -f_fix_sin(Angle);

    b->ignore = false;
}

static inline int ballRadius(const OBall* Ball)
{
    return g_ballsData[Ball->id].radius;
}

static inline FFix ballRadiusf(const OBall* Ball)
{
    return f_fix_fromInt(ballRadius(Ball));
}

static int ballCheckWalls(const OBall* Ball, FVecFix Coords)
{
    FVecInt coords = f_vecfix_toInt(Coords);
    FVecInt up = {coords.x, coords.y - ballRadius(Ball)};
    FVecInt down = {coords.x, coords.y + ballRadius(Ball)};
    FVecInt left = {coords.x - ballRadius(Ball), coords.y};
    FVecInt right = {coords.x + ballRadius(Ball), coords.y};

    int ret = 0;

    if(n_map_test(left) || n_map_test(right)) {
        ret |= 1;
    }

    if(n_map_test(up) || n_map_test(down)) {
        ret |= 2;
    }

    return ret;
}

static inline bool collideBoxes(int X1, int Y1, int W1, int H1, int X2, int Y2, int W2, int H2)
{
    return !(Y1 >= Y2 + H2 || Y2 >= Y1 + H1 || X1 >= X2 + W2 || X2 >= X1 + W1);
}

static inline bool circleAndCirclef(FVecFix Coords1, FFix R1, FVecFix Coords2, FFix R2)
{
    const int64_t x = Coords1.x - Coords2.x;
    const int64_t y = Coords1.y - Coords2.y;
    const int64_t r = R1 + R2;

    return x * x + y * y < r * r;
}

static void ball_tick_move(OBall* Ball)
{
    Ball->hitWall = false;
    Ball->hitBall = false;
    Ball->committed = false;

    for(int i = O_BALL_HISTORY_LEN - 1; i--; ) {
        Ball->coordsHistory[i + 1] = Ball->coordsHistory[i];
    }

    Ball->coordsHistory[0] = f_vecfix_toInt(Ball->coords);

    Ball->coordsNext.x = Ball->coords.x + Ball->velocity.x;
    Ball->coordsNext.y = Ball->coords.y + Ball->velocity.y;

    Ball->velocityNext = Ball->velocity;
}

static void ball_tick_validate(int Index)
{
    OBall* b1 = &g_balls[Index];

    int walls = ballCheckWalls(b1, b1->coordsNext);

    if(walls & 1) {
        b1->velocityNext.x *= -1;
        b1->hitWall = true;
    }

    if(walls & 2) {
        b1->velocityNext.y *= -1;
        b1->hitWall = true;
    }

    for(int i = Index + 1; i < g_tail; i++) {
        OBall* b2 = &g_balls[i];

        bool collide = circleAndCirclef(b1->coordsNext,
                                        ballRadiusf(b1),
                                        b2->coordsNext,
                                        ballRadiusf(b2));

        if(collide) {
            if(!b1->ignore && !b2->ignore) {
                b1->velocityNext.x += b2->velocity.x;
                b1->velocityNext.y += b2->velocity.y;

                b2->velocityNext.x += b1->velocity.x;
                b2->velocityNext.y += b1->velocity.y;
            } else if(b1->ignore && b2->ignore) {
                b1->velocityNext.x = -b2->velocity.x;
                b1->velocityNext.y = -b2->velocity.y;
            }

            if(!b2->ignore || b1->ignore) {
                b1->hitBall = true;
            }

            if(!b1->ignore || b2->ignore) {
                b2->hitBall = true;
            }
        }
    }
}

static void ball_tick_commit_1(OBall* Ball)
{
    if(!Ball->hitWall && Ball->hitBall && !Ball->ignore) {
        Ball->velocityNext.x -= Ball->velocity.x;
        Ball->velocityNext.y -= Ball->velocity.y;
    }

    if(Ball->velocityNext.x == 0 && Ball->velocityNext.y == 0) {
        Ball->velocityNext = Ball->velocity;
    }

    unsigned angle = f_fix_atan(
                        0, 0, Ball->velocityNext.x, Ball->velocityNext.y);

    Ball->velocity.x = +f_fix_cos(angle);
    Ball->velocity.y = -f_fix_sin(angle);

    if(!Ball->hitWall) {
        if(!Ball->hitBall) {
            Ball->coords = Ball->coordsNext;
            Ball->committed = true;
            Ball->ignore = false;
        } else if(Ball->ignore) {
            Ball->coords = Ball->coordsNext;
        }
    }
}

static bool ballCommitValid(const OBall* Ball, FVecFix Coords)
{
    if(ballCheckWalls(Ball, Coords)) {
        return false;
    }

    for(int i = 0; i < g_tail; i++) {
        OBall* b = &g_balls[i];

        if(b == Ball || !b->committed) {
            continue;
        }

        bool collide = circleAndCirclef(Coords,
                                        ballRadiusf(Ball),
                                        b->coords,
                                        ballRadiusf(b));

        if(collide) {
            return false;
        }
    }

    return true;
}

static void ball_tick_commit_2(OBall* Ball)
{
    if(Ball->committed || Ball->ignore) {
        return;
    }

    FVecFix vel = Ball->velocity;

    FVecFix nextCoords[] = {
        Ball->coords,
        {Ball->coords.x + vel.x / 4, Ball->coords.y + vel.y / 4},
        {Ball->coords.x - vel.x / 4, Ball->coords.y - vel.y / 4},
        {Ball->coords.x + vel.x / 2, Ball->coords.y + vel.y / 2},
        {Ball->coords.x - vel.x / 2, Ball->coords.y - vel.y / 2},
        {Ball->coords.x + vel.x / 1, Ball->coords.y + vel.y / 1},
        {Ball->coords.x - vel.x / 1, Ball->coords.y - vel.y / 1},
    };

    for(unsigned i = 0; i < F_ARRAY_LEN(nextCoords); i++) {
        if(ballCommitValid(Ball, nextCoords[i])) {
            Ball->coords = nextCoords[i];
            Ball->committed = true;

            return;
        }
    }

    if(!Ball->hitWall) {
        Ball->coords = Ball->coordsNext;
    }

    Ball->ignore = true;
}

void o_ball_tick(void)
{
    for(int i = 0; i < g_tail; i++) {
        ball_tick_move(&g_balls[i]);
    }

    for(int i = 0; i < g_tail; i++) {
        ball_tick_validate(i);
    }

    for(int i = 0; i < g_tail; i++) {
        ball_tick_commit_1(&g_balls[i]);
    }

    for(int i = 0; i < g_tail; i++) {
        ball_tick_commit_2(&g_balls[i]);
    }
}

static void ball_draw_trail(const OBall* Ball)
{
    for(int i = O_BALL_HISTORY_LEN; i--; ) {
        f_color_alphaSet(
            O_BALL_TRAIL_ALPHA - O_BALL_TRAIL_ALPHA * i / O_BALL_HISTORY_LEN);

        f_sprite_blit(*g_ballsData[Ball->id].sprite,
                      0,
                      Ball->coordsHistory[i].x,
                      Ball->coordsHistory[i].y);
    }
}

static void ball_draw_main(const OBall* Ball)
{
    FVecInt screen = f_vecfix_toInt(Ball->coords);

    f_sprite_blit(*g_ballsData[Ball->id].sprite, 0, screen.x, screen.y);
}

void o_ball_draw(void)
{
    UColorId color = (f_fps_ticksGet() & 0x8)
                        ? U_COLOR_BALL_YELLOW_1 : U_COLOR_BALL_YELLOW_2;

    f_align_set(F_ALIGN_X_CENTER, F_ALIGN_Y_CENTER);
    f_color_blendSet(F_COLOR_BLEND_ALPHA_MASK);
    f_color_colorSetIndex(color);

    for(int i = 0; i < g_tail; i++) {
        ball_draw_trail(&g_balls[i]);
    }

    f_color_colorSetIndex(U_COLOR_BALL_YELLOW_2);
    f_color_alphaSet(F_COLOR_ALPHA_MAX);

    for(int i = 0; i < g_tail; i++) {
        ball_draw_main(&g_balls[i]);
    }
}

bool o_ball_checkArea(FVecInt Start, FVecInt Dim)
{
    FVecFix start = f_vecint_toFix(Start);
    FVecFix dim = f_vecint_toFix(Dim);

    return o_ball_checkArea2(start.x, start.y, dim.x, dim.y);
}

bool o_ball_checkArea2(FFix X, FFix Y, FFix W, FFix H)
{
    for(int i = 0; i < g_tail; i++) {
        const OBall* b = &g_balls[i];

        FFix rad = f_fix_fromInt(ballRadius(b));
        FFix diameter = rad * 2 + F_FIX_ONE;

        bool hit = collideBoxes(b->coords.x - rad,
                                b->coords.y - rad,
                                diameter,
                                diameter,
                                X,
                                Y,
                                W,
                                H);

        if(hit) {
            return true;
        }
    }

    return false;
}
