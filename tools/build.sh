#!/bin/bash

target=target/wolf
src=src/main.cpp

g++ -std=c++23 -o $target $src || exit 1

#eval $target
