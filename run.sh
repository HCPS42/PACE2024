#!/bin/bash

# cp tests/input/heuristic/$1.gr input.txt

# I used the following command to test the code locally
# g++ -g -O2 -std=gnu++2a -DLOCAL heuristic.cpp -o PACE2024

g++ -g -O2 -std=gnu++2a heuristic.cpp -o PACE2024

./PACE2024 < input.txt
