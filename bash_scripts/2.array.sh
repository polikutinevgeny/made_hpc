#!/usr/bin/env bash

array=(1 2 3 4 5 6 7 8 9 10 eleven twelve thirteen fourteen fifteen)

for i in ${array[*]}
do
    echo $i
done
