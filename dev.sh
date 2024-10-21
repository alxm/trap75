#!/bin/sh
geany -i build/make/* `find trap75 -type f` dev.sh README.md .gitignore &
cd build/make
