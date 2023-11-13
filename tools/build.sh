#!/bin/bash

DEBUG=false
SRC=src/main.cpp


if [ "$DEBUG" = true ]; then
    TARGET_DIR=target/debug
    OPTIMIZATION="-Og -g"
    SANITIZERS="-fsanitize=undefined,thread"
    echo "creating debug build"
else
    echo "creating release build"
    TARGET_DIR=target/release
    OPTIMIZATION="-O2"
    SANITIZERS=""
fi

mkdir -p $TARGET_DIR

clang++ \
    -std=c++20 \
    -Wall -Wextra -Wpedantic \
    -pthread \
    "$OPTIMIZATION" \
    "$SANITIZERS" \
    -o "$TARGET_DIR"/wolf "$SRC" \
    || exit 1

echo "build complete"
