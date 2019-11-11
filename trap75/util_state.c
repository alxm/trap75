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

#include "util_state.h"

#include "obj_camera.h"
#include "obj_game.h"
#include "state_intro.h"
#include "state_next.h"
#include "state_over.h"
#include "state_play.h"
#include "state_start.h"
#include "state_title.h"
#include "util_input.h"
#include "util_light.h"
#include "util_save.h"
#include "util_sound.h"
#include "util_swipe.h"
#include "util_timer.h"

typedef struct {
    ZStateInit* init;
    ZStateTick* tick;
    ZStateDraw* draw;
    ZStateFree* free;
} ZState;

static const ZState g_states[Z_STATE_NUM] = {
    [Z_STATE_INTRO] = {
        s_intro_init,
        s_intro_tick,
        s_intro_draw,
        NULL,
    },
    [Z_STATE_NEXT] = {
        s_next_init,
        s_next_tick,
        s_next_draw,
        s_next_free,
    },
    [Z_STATE_OVER] = {
        s_over_init,
        s_over_tick,
        s_over_draw,
        s_over_free,
    },
    [Z_STATE_PLAY] = {
        NULL,
        s_play_tick,
        s_play_draw,
        NULL,
    },
    [Z_STATE_START] = {
        s_start_init,
        s_start_tick,
        s_start_draw,
        NULL,
    },
    [Z_STATE_TITLE] = {
        s_title_init,
        s_title_tick,
        s_title_draw,
        NULL,
    }
};

static struct {
    ZStateId current;
    ZStateId next;
} g_state = {
    Z_STATE_INVALID,
    Z_STATE_INVALID,
};

void z_state_setup(void)
{
    z_save_setup();

    z_graphics_setup();
    z_sound_setup();

    z_input_reset();
    z_light_reset();

    #if Z_DEBUG
        z_state_set(Z_STATE_TITLE);
    #else
        z_state_set(Z_STATE_INTRO);
    #endif
}

static void checkNewState(void)
{
    if(g_state.next == Z_STATE_INVALID || z_swipe_running()) {
        return;
    }

    if(g_state.current != Z_STATE_INVALID && g_states[g_state.current].free) {
        g_states[g_state.current].free();
    }

    g_state.current = g_state.next;
    g_state.next = Z_STATE_INVALID;

    if(g_states[g_state.current].init) {
        g_states[g_state.current].init();
    }
}

void z_state_tick(void)
{
    z_timer_tick();
    z_swipe_tick();
    z_light_tick();
    z_input_tick();

    checkNewState();

    if(g_states[g_state.current].tick) {
        g_states[g_state.current].tick();

        checkNewState();
    }
}

void z_state_draw(void)
{
    if(g_states[g_state.current].draw) {
        g_states[g_state.current].draw();
    }

    z_swipe_draw();
    z_light_draw();
}

ZStateId z_state_getCurrent(void)
{
    return g_state.current;
}

ZStateId z_state_getNext(void)
{
    return g_state.next;
}

void z_state_set(ZStateId NewState)
{
    if(g_state.next != Z_STATE_INVALID) {
        return;
    }

    g_state.next = NewState;

    z_swipe_stop();
}

bool z_state_changed(void)
{
    return g_state.next != Z_STATE_INVALID;
}
