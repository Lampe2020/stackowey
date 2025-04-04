#!/usr/bin/env bash
# -*- coding: utf-8 -*-
if [ "$1" = 'w' ]
then
    export WINEDEBUG=-all
    rm -v stackowey.exe
    printf "Compiling...\n"
    #TODO: Reencode source to windows-1258 encoding first? 
    wine C:\\Dev-Cpp\\bin\\g++.exe -finput-charset=UTF-8 -fexec-charset=UTF-8 -pipe -o stackowey.exe --debug main.cpp &&
    printf "Running...\n" &&
    wineconsole stackowey.exe
    printf 'Ended with exit code %s\n' "$?"
else
    rm -v stackowey
    printf "Compiling...\n"
    g++ -o stackowey --debug -pipe main.cpp &&
    printf "Running...\n" &&
    ./stackowey
    printf 'Ended with exit code %s\n' "$?"
fi
