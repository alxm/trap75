#!/bin/sh
geany -i build/make/* trap75/* dev.sh README.md .gitignore &
gimp -n assets/gfx/palette.png &
cd build/make
