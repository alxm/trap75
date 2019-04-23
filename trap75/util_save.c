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

#include "util_save.h"

#define Z_SAVE_FILE_NAME A_CONFIG_PATH_STORAGE_PREFIX "trap75.sav"

typedef struct {
    uint32_t version;
    uint32_t hiscore;
} ZSaveFile;

ZSaveFile g_save;

void z_save_setup(void)
{
    g_save.version = 1;

    z_file_readOnce(Z_SAVE_FILE_NAME, &g_save, sizeof(g_save));
}

unsigned z_save_hiscoreGet(void)
{
    return g_save.hiscore;
}

void z_save_hiscoreSet(unsigned Score)
{
    if(Score > g_save.hiscore) {
        g_save.hiscore = Score;

        z_file_writeOnce(Z_SAVE_FILE_NAME, &g_save, sizeof(g_save));
    }
}
