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

#include "data_assets.h"

//
// Workaround for Arduino not building code from subdirs
//
#if F_CONFIG_SYSTEM_GAMEBUINO
    #include "faur_gen/gfx/assets/gfx/alxm.png.c"
    #include "faur_gen/gfx/assets/gfx/alxm_footer.png.c"
    #include "faur_gen/gfx/assets/gfx/alxm_wing.png.c"
    #include "faur_gen/gfx/assets/gfx/ball1.png.c"
    #include "faur_gen/gfx/assets/gfx/ball2.png.c"
    #include "faur_gen/gfx/assets/gfx/ball3.png.c"
    #include "faur_gen/gfx/assets/gfx/ball4.png.c"
    #include "faur_gen/gfx/assets/gfx/cursor.png.c"
    #include "faur_gen/gfx/assets/gfx/font_aa_3x5.png.c"
    #include "faur_gen/gfx/assets/gfx/icon_heart.png.c"
    #include "faur_gen/gfx/assets/gfx/icon_level.png.c"
    #include "faur_gen/gfx/assets/gfx/palette.png.c"
    #include "faur_gen/gfx/assets/gfx/space1.png.c"
    #include "faur_gen/gfx/assets/gfx/title.png.c"
    #include "faur_gen/gfx/assets/gfx/title_glow.png.c"

    #include "faur_gen/sfx/assets/sfx/swipe_hide.wav.c"
    #include "faur_gen/sfx/assets/sfx/swipe_show.wav.c"
#endif
