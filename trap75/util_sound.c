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

#include "util_sound.h"

#include "generated/data_sfx_pressed_a.h"
#include "generated/data_sfx_swipe_hide.h"
#include "generated/data_sfx_swipe_show.h"

void z_sound_setup(void)
{
    z_sfx_load(Z_SFX_PRESSED_A, pressed_a);
    z_sfx_load(Z_SFX_SWIPE_HIDE, swipe_hide);
    z_sfx_load(Z_SFX_SWIPE_SHOW, swipe_show);
}
