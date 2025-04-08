#!/usr/bin/env bash
# -*- coding: utf-8 -*-
if [ "$1" = 'w' ]
then
    export WINEDEBUG=-all
    rm -v stackowey.exe
    printf "Compiling...\n"
    #TODO: Reencode source to windows-1258 encoding first? 
    wine C:\\Dev-Cpp\\bin\\g++.exe -std=c++23 -finput-charset=UTF-8 -fexec-charset=UTF-8 -pipe -o stackowey.exe --debug main.cpp &&
    printf "Running...\n" &&
    wineconsole stackowey.exe -d
    printf 'Ended with exit code %s\n' "$?"
else
    rm -v stackowey
    printf "Compiling...\n"
    g++ -std=c++23 -o stackowey --debug -pipe main.cpp &&
    printf "Running...\n" &&
    ./stackowey -d
    printf 'Ended with exit code %s\n' "$?"
fi
