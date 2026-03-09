#!/bin/bash

echo "== Serial =="
serial_time=$(./sp | grep Elapsed | awk '{print $2}')
echo "T_seq = $serial_time"

echo
printf "%-10s %-8s %-12s %-12s %-12s\n" "Version" "Threads" "Time(s)" "Speedup" "Efficiency"

for exe in pp1 pp2 pp3; do
  for t in 2 4 8 16; do
    tp=$(./$exe $t | grep Elapsed | awk '{print $2}')
    speedup=$(awk "BEGIN {print $serial_time / $tp}")
    efficiency=$(awk "BEGIN {print ($serial_time / $tp) / $t}")
    printf "%-10s %-8s %-12s %-12s %-12s\n" "$exe" "$t" "$tp" "$speedup" "$efficiency"
  done
done