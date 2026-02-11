#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$REPO_ROOT/build/bin/syft4golog"

RESULTS="$REPO_ROOT/results/results_tireworld_simplified_ground.csv"
TIMEOUT_SECS=600

cd "$REPO_ROOT/build/bin"

for line_dim in $(seq 10 15); do
  DOMAIN="$REPO_ROOT/benchmarks/tireworld_simplified_ground/domain_n${line_dim}.pddl"
  PROBLEM="$REPO_ROOT/benchmarks/tireworld_simplified_ground/p${line_dim}.pddl"

  for expertise in 1 2 3; do
    GOLOG="$REPO_ROOT/benchmarks/tireworld_simplified_ground/prog_${expertise}_${line_dim}.golog"

    for alg in 1 2; do
      echo "=== line_dim=$line_dim expertise=$expertise alg=$alg ==="

      timeout "$TIMEOUT_SECS" "$BIN" \
        -d "$DOMAIN" \
        -i "$PROBLEM" \
        -g "$GOLOG" \
        -a "$alg" \
        -r "$RESULTS" \
        || echo "Run failed (line_dim=$line_dim, exp=$expertise, alg=$alg) with exit code $?"
    done
  done
done

echo "Done. Results at: $RESULTS"
