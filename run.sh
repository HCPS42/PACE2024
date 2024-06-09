#!/bin/bash

cp tests/param/$1.gr input.txt

# Compile main.cpp with g++
g++ -g -O2 -std=gnu++2a -DLOCAL main.cpp -o PACE2024

# Run the executable
./PACE2024 < input.txt
