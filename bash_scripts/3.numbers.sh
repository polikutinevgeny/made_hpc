#!/usr/bin/env bash

left=100
right=0.5
output=$(echo $left + $right | bc)
echo $output
