#!/usr/bin/env bash
# -*- coding: utf-8 -*-
rm -v stackowey
printf "Compiling...\n"
g++ -std=c++23 -o stackowey --debug -pipe main.cpp &&
printf "Running...\n" &&
./stackowey -d
printf '\n-> Ended with exit code %s\n' "$?"
