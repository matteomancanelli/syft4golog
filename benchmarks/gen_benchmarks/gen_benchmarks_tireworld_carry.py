import os
import math

OUT_DIR = "./benchmarks/tireworld_carry"

def ensure_outdir():
    os.makedirs(OUT_DIR, exist_ok=True)

def bits_needed(n):
    return max(1, math.ceil(math.log2(n)))

def int_to_bits(x, k):
    return [(x >> i) & 1 for i in range(k)]

def bits_precond(x, k):
    # conjunction over bit_i true/false matching x
    lits = []
    for i, b in enumerate(int_to_bits(x, k)):
        if b == 1:
            lits.append(f"(bit{i})")
        else:
            lits.append(f"(not (bit{i}))")
    return " ".join(lits)

def bits_effect_set(x, k):
    # set bits to x deterministically
    eff = []
    for i, b in enumerate(int_to_bits(x, k)):
        if b == 1:
            eff.append(f"(bit{i})")
        else:
            eff.append(f"(not (bit{i}))")
    return " ".join(eff)

def write_domain(n, domain_path):
    k = bits_needed(n)
    lines = []
    lines.append("(define (domain tire-carry)\n")
    lines.append("  (:requirements :strips :non-deterministic)\n")
    lines.append("  (:predicates\n")
    for i in range(k):
        lines.append(f"    (bit{i})\n")
    lines.append("    (not-flattire)\n")
    lines.append("    (has-spare)\n")
    lines.append("  )\n\n")

    # pickup spare: always available, ensures realizability
    lines.append("  (:action pickup_spare\n")
    lines.append("    :parameters ()\n")
    lines.append("    :precondition (not (has-spare))\n")
    lines.append("    :effect (has-spare)\n")
    lines.append("  )\n\n")

    # change tire: consume spare, restore not-flattire
    lines.append("  (:action changetire\n")
    lines.append("    :parameters ()\n")
    lines.append("    :precondition (and (has-spare))\n")
    lines.append("    :effect (and (not-flattire) (not (has-spare)))\n")
    lines.append("  )\n\n")

    # move forward/backward actions for each concrete position
    for p in range(n):
        if p < n-1:
            p_to = p+1
            lines.append(f"  (:action move_fwd_p{p}\n")
            lines.append("    :parameters ()\n")
            lines.append(f"    :precondition (and {bits_precond(p,k)} (not-flattire))\n")
            lines.append("    :effect (and\n")
            lines.append(f"      {bits_effect_set(p_to,k)}\n")
            lines.append("      (oneof (and) (not (not-flattire)))\n")
            lines.append("    )\n")
            lines.append("  )\n\n")
        if p > 0:
            p_to = p-1
            lines.append(f"  (:action move_bwd_p{p}\n")
            lines.append("    :parameters ()\n")
            lines.append(f"    :precondition (and {bits_precond(p,k)} (not-flattire))\n")
            lines.append("    :effect (and\n")
            lines.append(f"      {bits_effect_set(p_to,k)}\n")
            lines.append("      (oneof (and) (not (not-flattire)))\n")
            lines.append("    )\n")
            lines.append("  )\n\n")

    lines.append(")\n")

    with open(domain_path, "w") as f:
        f.write("".join(lines))

def write_problem(n, problem_path):
    k = bits_needed(n)
    goal = bits_precond(n-1, k)

    lines = []
    lines.append(f"(define (problem tire_carry_{n})\n")
    lines.append("  (:domain tire-carry)\n")
    lines.append("  (:init\n")
    # start at 0: all bits false
    for i in range(k):
        lines.append(f"    (not (bit{i}))\n")
    lines.append("    (not-flattire)\n")
    # has-spare initially false
    lines.append("  )\n")
    lines.append(f"  (:goal (and {goal}))\n")
    lines.append(")\n")

    with open(problem_path, "w") as f:
        f.write("".join(lines))

# ------------------ Golog programs ------------------

def goal_test(n):
    k = bits_needed(n)
    # your golog uses [atom]? style; assume you already encode fluents as bit0, etc.
    # If your parser expects underscores, replace accordingly.
    conjuncts = []
    for i, b in enumerate(int_to_bits(n-1,k)):
        if b == 1:
            conjuncts.append(f"bit{i}")
        else:
            conjuncts.append(f"!bit{i}")  # if your syntax uses negation; otherwise adapt
    # If you don't support negated fluents in tests, see note below.
    return "[" + " && ".join(conjuncts) + "]?"

def write_prog1(n):
    # unstructured: any move action, plus pickup_spare and changetire
    moves = []
    for p in range(n):
        if p < n-1:
            moves.append(f"move_fwd_p{p}_")
        if p > 0:
            moves.append(f"move_bwd_p{p}_")
    move_somewhere = "(" + " | ".join(moves) + ")"
    pick_action = "(" + " | ".join([move_somewhere, "pickup_spare_", "changetire_"]) + ")*"
    return pick_action + " ; " + goal_test(n)

def write_prog2(n):
    moves = []
    for p in range(n):
        if p < n-1:
            moves.append(f"move_fwd_p{p}_")
        if p > 0:
            moves.append(f"move_bwd_p{p}_")
    move_somewhere = "(" + " | ".join(moves) + ")"
    loop = f"((nil | pickup_spare_) ; (nil | {move_somewhere}) ; (nil | changetire_))*"
    return loop + " ; " + goal_test(n)

def write_prog3(n):
    # structured: go forward only, always maintain spare to guarantee progress
    parts = []
    for p in range(n-1):
        parts.append(f"(pickup_spare_ ; move_fwd_p{p}_ ; (nil | changetire_) ; pickup_spare_)")
    return "(" + " ; ".join(parts) + ") ; " + goal_test(n)

def write_program(n, expertise):
    if expertise == 1: return write_prog1(n)
    if expertise == 2: return write_prog2(n)
    if expertise == 3: return write_prog3(n)
    raise ValueError()

def main():
    ensure_outdir()
    for n in range(2, 51):
        write_domain(n, os.path.join(OUT_DIR, "domain.pddl"))
        write_problem(n, os.path.join(OUT_DIR, f"p{n}.pddl"))
        for e in (1,2,3):
            with open(os.path.join(OUT_DIR, f"prog_{e}_{n}.golog"), "w") as f:
                f.write(write_program(n, e))

if __name__ == "__main__":
    main()
