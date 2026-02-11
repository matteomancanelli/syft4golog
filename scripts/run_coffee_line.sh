#!/usr/bin/env bash
set -euo pipefail

# Always run from repo root (script location) robustly
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$REPO_ROOT/build/bin/syft4golog"

DOMAIN="$REPO_ROOT/benchmarks/robot_coffee_line/domain.pddl"
RESULTS="$REPO_ROOT/results/results_robot_coffee_line.csv"

TIMEOUT_SECS=600

cd "$REPO_ROOT/build/bin"

for num_locations in $(seq 8 10); do
  PROBLEM="$REPO_ROOT/benchmarks/robot_coffee_line/p${num_locations}.pddl"

  for expertise in 3; do
    GOLOG="$REPO_ROOT/benchmarks/robot_coffee_line/prog_${expertise}_${num_locations}.golog"

    for alg in 1 2; do
      echo "=== num_locations=$num_locations expertise=$expertise alg=$alg ==="

      timeout "$TIMEOUT_SECS" "$BIN" \
        -d "$DOMAIN" \
        -i "$PROBLEM" \
        -g "$GOLOG" \
        -a "$alg" \
        -r "$RESULTS" \
        || echo "Run failed (num_locations=$num_locations, exp=$expertise, alg=$alg) with exit code $?"
    done
  done
done

echo "Done. Results at: $RESULTS"
