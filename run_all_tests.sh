#!/bin/bash

g++ -g -O2 -std=gnu++2a heuristic.cpp -o heuristic
g++ -g -O2 -std=gnu++2a parametrized.cpp -o parametrized

mkdir -p tests/output

test_sets=("exact" "heuristic" "parametrized")

for test_set in "${test_sets[@]}"; do
  for i in $(seq 1 100); do
    mkdir -p tests/output/$test_set
    output_file="tests/output/$test_set/${i}.txt"
    input_file="tests/input/$test_set/${i}.gr"
    echo "Processing $input_file"
    if [ "$test_set" == "parametrized" ]; then
      ./parametrized < $input_file > $output_file
    else
      ./heuristic < $input_file > $output_file
    fi
  done
done

