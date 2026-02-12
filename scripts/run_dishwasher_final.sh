#!/usr/bin/env bash
set -euo pipefail

# Always run from repo root (script location) robustly
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$REPO_ROOT/build/bin/syft4golog"

DOMAIN="$REPO_ROOT/benchmarks/dishwasher_final/domain.pddl"
RESULTS="$REPO_ROOT/results/results_dishwasher_final.csv"

TIMEOUT_SECS=600

cd "$REPO_ROOT/build/bin"

for num_rooms in $(seq 1 1); do
  for num_dishes in $(seq 1 9); do
    PROBLEM="$REPO_ROOT/benchmarks/dishwasher_final/p_r${num_rooms}_d${num_dishes}.pddl"

    for expertise in 1 2 3; do
      GOLOG="$REPO_ROOT/benchmarks/dishwasher_final/prog_${expertise}_r${num_rooms}_d${num_dishes}.golog"

        for alg in 1 2; do
        #if [[ "$expertise" -eq 2 && "$alg" -eq 2 ]]; then
        #  echo "Skipping alg=2 for expertise=2..."
        #  continue
        #fi
        
        echo "=== num_rooms=$num_rooms num_dishes=$num_dishes expertise=$expertise alg=$alg ==="

        timeout "$TIMEOUT_SECS" "$BIN" \
            -d "$DOMAIN" \
            -i "$PROBLEM" \
            -g "$GOLOG" \
            -a "$alg" \
            -r "$RESULTS" \
            || echo "Run failed (num_rooms=$num_rooms, num_dishes=$num_dishes, exp=$expertise, alg=$alg) with exit code $?"
        done
    done
  done
done

echo "Done. Results at: $RESULTS"
