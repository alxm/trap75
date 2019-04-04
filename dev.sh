#!/bin/sh
geany -i make/* trap75/* dev.sh README.md &
arduino trap75/trap75.ino &
gimp -n assets/gfx/palette.png &
cd make
