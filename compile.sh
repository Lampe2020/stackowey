#!/usr/bin/env bash
# -*- coding: utf-8 -*-
rm -v stackowey
printf "Compiling...\n"
if [ "$1" = "-d" ]
then
    g++ --debug -std=c++23 -o stackowey -pipe main.cpp &&
    printf "Running...\n" &&
    ./stackowey -d
else
    g++ -Wall -Werror -O3 -ffast-math -std=c++23 -o stackowey -pipe main.cpp &&
    printf "Running...\n" &&
    ./stackowey
fi
printf '\n-> Ended with exit code %s\n' "$?"
