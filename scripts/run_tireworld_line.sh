#!/usr/bin/env bash
set -euo pipefail

# Always run from repo root (script location) robustly
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$REPO_ROOT/build/bin/syft4golog"

DOMAIN="$REPO_ROOT/benchmarks/tireworld_line/domain.pddl"
RESULTS="$REPO_ROOT/results/results_tireworld_line.csv"

TIMEOUT_SECS=600

cd "$REPO_ROOT/build/bin"

for line_dim in $(seq 3 12); do
  PROBLEM="$REPO_ROOT/benchmarks/tireworld_line/p${line_dim}.pddl"

  for expertise in 1 2 3; do
    GOLOG="$REPO_ROOT/benchmarks/tireworld_line/prog_${expertise}_${line_dim}.golog"

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
