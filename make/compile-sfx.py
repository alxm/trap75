#!/usr/bin/env python3

"""
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
"""

import subprocess
import sys

def main(SfxName, UniqueName):
    ret = subprocess.run(['ffmpeg',
                            '-loglevel', 'quiet',
                            '-i', SfxName,
                            '-ac', '1',
                            '-ar', '22050',
                            '-acodec', 'pcm_u8',
                            '-f', 'u8',
                            'pipe:1'],
                         stdout = subprocess.PIPE)

    if ret.returncode != 0:
        sys.exit(ret.returncode)

    def formatBytes8(Bytes):
        formattedBytes = ''

        for index, byte in enumerate(Bytes):
            """if index % 15 == 0:
                formattedBytes += '\n    '"""

            formattedBytes += '0x{:0>2x},'.format(byte)

        return formattedBytes

    contents = """\
/*
    Copyright Alex Margarit <alex@alxm.org>
    This file is part of Trap75, a video game.
    Generated by assets/sfx/compile.py from {path}
*/

#if Z_PLATFORM_META
static const uint32_t z_data_sfx_{name}_size = {size};

static const uint8_t z_data_sfx_{name}_buffer[] = {{
    // Raw sfx bytes
    {metaSfxBytes}
}};
#endif // if Z_PLATFORM_META"""

    print(contents.format(path = SfxName,
                          name = UniqueName,
                          size = len(ret.stdout),
                          metaSfxBytes = formatBytes8(ret.stdout)))

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('Usage: {} Sfx.wav UniqueName'.format(sys.argv[0]))
        sys.exit(1)
    else:
        main(sys.argv[1], sys.argv[2])
