#!/bin/bash

CSD=$(dirname "$0")

rm perf.data
perf record -e cache-misses -e cache-references -o perf.data $CSD/../target/release/wolf || exit 1
perf report
