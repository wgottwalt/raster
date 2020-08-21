#!/bin/sh

FILES="/tmp/to_remove.$$"

find | grep "CMakeCache.txt" > $FILES
find | grep "CMakeFiles" >> $FILES
find | grep "Makefile" >> $FILES
find | grep "cmake_install.cmake" >> $FILES

while read FILE
do
    rm -rf $FILE
done < $FILES
rm $FILES

scripts/CodeCounter.sh
