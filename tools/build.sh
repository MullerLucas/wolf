#!/bin/bash

IS_DEBUG_BUILD=true
SRC=src/main.cpp

# -----------------------------------------------

# parse command line arguments
POSITIONAL_ARGS=()

while [[ $# -gt 0 ]]; do
  case $1 in
    -r|--release)
      IS_DEBUG_BUILD=false
      shift
      ;;
    *)
      POSITIONAL_ARGS+=("$1") # save positional arg
      shift
      ;;
  esac
done

set -- "${POSITIONAL_ARGS[@]}" # restore positional parameters

# -----------------------------------------------

# set variables based on build type
if [ "$IS_DEBUG_BUILD" = true ]; then
    TARGET_DIR=target/debug
    OPTIMIZATION="-Og"
    DEBUG_SYMBOLS="-g"
    SANITIZERS="-fsanitize=undefined,thread"
    echo "creating debug build"
else
    echo "creating release build"
    TARGET_DIR=target/release
    OPTIMIZATION="-O2"
    SANITIZERS=""
fi

# ensure target directory exists
mkdir -p $TARGET_DIR

# build
clang++ \
    -std=c++20 \
    -Wall -Wextra -Wpedantic \
    -pthread \
    "$OPTIMIZATION" \
    "$DEBUG_SYMBOLS" \
    "$SANITIZERS" \
    -o "$TARGET_DIR"/wolf "$SRC" \
    || exit 1

echo "build complete"
