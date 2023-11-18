#!/bin/bash

./tools/build.sh -r && flamegraph -- ./target/release/wolf -b && firefox flamegraph.svg
