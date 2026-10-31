#!/usr/bin/env bash
set -euo pipefail

STRATEGY="${1:-flat}"
BIN="${BIN:-build/casino}"
OUT="runs"

mkdir -p "$OUT"
journals=()
for seed in 1 2 3 4 5; do
    journal="$OUT/${STRATEGY}_seed${seed}.csv"
    "$BIN" --mode bot --strategy "$STRATEGY" --seed "$seed" \
        --spins 200 --loss-limit 500 --win-goal 500 --journal "$journal"
    echo
    journals+=("$journal")
done

python3 tools/plot.py "${journals[@]}" --out "$OUT"
