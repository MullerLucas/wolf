#!/bin/bash

CSD=$(dirname "$0")

rm perf.data
perf record -e cache-misses -e cache-references -o perf.data $CSD/../target/release/wolf || exit 1
perf report

# perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,LLC-prefetches
