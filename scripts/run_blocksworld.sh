#!/usr/bin/env bash
set -euo pipefail

# Always run from repo root (script location) robustly
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$REPO_ROOT/build/bin/syft4golog"

DOMAIN="$REPO_ROOT/benchmarks/blocksworld/domain.pddl"
RESULTS="$REPO_ROOT/results/results_blocksworld.csv"

TIMEOUT_SECS=600

cd "$REPO_ROOT/build/bin"

for num_blocks in $(seq 3 8); do
  PROBLEM="$REPO_ROOT/benchmarks/blocksworld/p${num_blocks}.pddl"

  for expertise in 1 2 3; do
    GOLOG="$REPO_ROOT/benchmarks/blocksworld/prog_${expertise}_${num_blocks}.golog"

    for alg in 1 2; do
      echo "=== num_blocks=$num_blocks expertise=$expertise alg=$alg ==="

      timeout "$TIMEOUT_SECS" "$BIN" \
        -d "$DOMAIN" \
        -i "$PROBLEM" \
        -g "$GOLOG" \
        -a "$alg" \
        -r "$RESULTS" \
        || echo "Run failed (num_blocks=$num_blocks, exp=$expertise, alg=$alg) with exit code $?"
    done
  done
done

echo "Done. Results at: $RESULTS"
