#!/bin/bash

mkdir project
mkdir project/doc project/include project/src
echo Picard Quentin > project/README
touch project/include/func.h project/src/main.c project/src/func.c
ls project -R --group-directories-first > contents.txt
cp -r project projectV2
rm -R project
tar -cf projectV2.tar projectV2