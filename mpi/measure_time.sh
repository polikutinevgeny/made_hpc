#!/usr/bin/env bash

for (( i = 1; i <= 8; i++ )); do
    echo -n "$i,"
    \time -f "%e" mpirun -np $i ./cmake-build-release/cell_automata
done
