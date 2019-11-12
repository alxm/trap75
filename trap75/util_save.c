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

#include "util_save.h"

#define Z_SAVE_FILE_NAME F_CONFIG_FILES_PREFIX "trap75.sav"

typedef struct {
    uint32_t version;
    uint32_t hiscore;
} ZSaveFile;

ZSaveFile g_save;

void z_save_setup(void)
{
    g_save.version = 1;

    FFile* f = f_file_new(Z_SAVE_FILE_NAME, F_FILE_READ);

    if(f) {
        f_file_read(f, &g_save, sizeof(g_save));
        f_file_free(f);
    }
}

unsigned z_save_hiscoreGet(void)
{
    return g_save.hiscore;
}

void z_save_hiscoreSet(unsigned Score)
{
    if(Score > g_save.hiscore) {
        g_save.hiscore = Score;

        FFile* f = f_file_new(Z_SAVE_FILE_NAME, F_FILE_WRITE);

        if(f) {
            f_file_write(f, &g_save, sizeof(g_save));
            f_file_free(f);
        }
    }
}
