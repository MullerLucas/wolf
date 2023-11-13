#!/bin/bash

CSD=$(dirname "$0")

valgrind --tool=drd "$CSD"/../target/release/wolf
