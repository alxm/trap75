#!/bin/sh
geany -i build/make/* trap75/* dev.sh README.md .gitignore &
arduino trap75/trap75.ino &
gimp -n assets/gfx/palette.png &
cd build/make
