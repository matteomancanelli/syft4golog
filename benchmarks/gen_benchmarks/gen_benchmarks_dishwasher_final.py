import os

def ensure_dir(path: str):
    os.makedirs(path, exist_ok=True)

def dish_home_room(i: int, num_rooms: int) -> int:
    """1-indexed room assignment, cyclic."""
    return ((i - 1) % num_rooms) + 1

# -----------------------
# PROBLEM GENERATION
# -----------------------
def create_problem(num_rooms: int, num_dishes: int) -> str:
    prob_name = f"dishwasher_r{num_rooms}_d{num_dishes}"

    rooms = " ".join([f"r{i}" for i in range(1, num_rooms + 1)])
    dishes = " ".join([f"d{i}" for i in range(1, num_dishes + 1)])

    lines = [
        f"(define (problem {prob_name})",
        "(:domain warehouse-domain)",
        "(:objects",
        f"  {dishes} - dish",
        f"  {rooms} - room",
        ")",
        "(:init",
        "  (emptyhand)",
        "  (robot-at r0)",
    ]

    # Place dishes cyclically over rooms; none are dirty initially.
    for i in range(1, num_dishes + 1):
        r = dish_home_room(i, num_rooms)
        lines.append(f"  (dish-at d{i} r{r})")

    lines.append(")")

    # Goal: all dishes are at r0 AND not dirty (and robot not holding anything).
    lines.append("(:goal (and")
    for i in range(1, num_dishes + 1):
        lines.append(f"  (dish-at d{i} r0)")
        #lines.append(f"  (not (dirty d{i}))")
    lines.append("))")
    lines.append(")")
    return "\n".join(lines)

def gen_problem(num_rooms: int, num_dishes: int, out_dir: str):
    ensure_dir(out_dir)
    with open(os.path.join(out_dir, f"p_r{num_rooms}_d{num_dishes}.pddl"), "w") as f:
        f.write(create_problem(num_rooms, num_dishes))

# -----------------------
# GOLOG PROGRAMS
# -----------------------
def golog_test_goal(num_dishes: int) -> str:
    # add !dirty_di as requested; also ensure we aren't holding anything
    tests = []
    for i in range(1, num_dishes + 1):
        tests.append(f"dish_at_d{i}_r0")
        #tests.append(f"!dirty_d{i}")
    return "[" + " && ".join(tests) + "]?"

def write_program_exp1(num_rooms: int, num_dishes: int) -> str:
    """
    Type 1 blueprint: pick whatever action; loop; check final condition.
    Minimal-change variant:
      - moves: r0<->ri
      - loads: ONLY from each dish's initial home room (keeps TS smaller / avoids unreachable combos)
      - try_unload at r0
      - wash
      - unload_clean at r0
    """
    move_actions = []
    for r in range(1, num_rooms + 1):
        move_actions.append(f"move_r0_r{r}")
        move_actions.append(f"move_r{r}_r0")
    move_somewhere = "(" + " | ".join(move_actions) + ")"

    # load only from initial placement room for each dish
    load_some = "(" + " | ".join(
        [f"load_d{i}_r{dish_home_room(i, num_rooms)}" for i in range(1, num_dishes + 1)]
    ) + ")"

    try_unload_some = "(" + " | ".join([f"try_unload_d{i}_r0" for i in range(1, num_dishes + 1)]) + ")"
    wash_some = "(" + " | ".join([f"wash_d{i}" for i in range(1, num_dishes + 1)]) + ")"
    unload_clean_some = "(" + " | ".join([f"unload_clean_d{i}_r0" for i in range(1, num_dishes + 1)]) + ")"

    pick_action = " | ".join([move_somewhere, load_some, try_unload_some, wash_some, unload_clean_some])
    loop_body = f"({pick_action})*"
    return f"{loop_body} ; {golog_test_goal(num_dishes)}"

def write_program_exp2(num_rooms: int, num_dishes: int) -> str:
    """
    Type 2: loop over “deliver some dish”.
    For dish i, go to its home room (where it initially is), load, return, try_unload at r0,
    then optionally wash+unload_clean if it became dirty.
    """
    deliver = []
    for i in range(1, num_dishes + 1):
        r = dish_home_room(i, num_rooms)
        deliver.append(
            f"(move_r0_r{r} ; load_d{i}_r{r} ; move_r{r}_r0 ; "
            f"try_unload_d{i}_r0 ; (nil | (wash_d{i} ; unload_clean_d{i}_r0)))"
        )
    loop_body = "(" + " | ".join(deliver) + ")*"
    return f"{loop_body} ; {golog_test_goal(num_dishes)}"

def write_program_exp3(num_rooms: int, num_dishes: int) -> str:
    """
    Type 3: sequentially deliver each dish with the same pattern as exp2.
    """
    steps = []
    for i in range(1, num_dishes + 1):
        r = dish_home_room(i, num_rooms)
        steps.append(
            f"(move_r0_r{r} ; load_d{i}_r{r} ; move_r{r}_r0 ; "
            f"try_unload_d{i}_r0 ; (nil | (wash_d{i} ; unload_clean_d{i}_r0)))"
        )
    body = "(" + " ; ".join(steps) + ")"
    return f"{body} ; {golog_test_goal(num_dishes)}"

def gen_program(num_rooms: int, num_dishes: int, expertise: int, out_dir: str):
    ensure_dir(out_dir)
    if expertise == 1:
        program_str = write_program_exp1(num_rooms, num_dishes)
    elif expertise == 2:
        program_str = write_program_exp2(num_rooms, num_dishes)
    elif expertise == 3:
        program_str = write_program_exp3(num_rooms, num_dishes)
    else:
        raise ValueError("Expertise level must be 1, 2, or 3.")

    with open(os.path.join(out_dir, f"prog_{expertise}_r{num_rooms}_d{num_dishes}.golog"), "w") as f:
        f.write(program_str)

# -----------------------
# DRIVER
# -----------------------
if __name__ == "__main__":
    out = "./benchmarks/dishwasher_final"
    for expertise in [1, 2, 3]:
        for R in range(1, 6):
            for D in range(1, 11):
                gen_problem(R, D, out_dir=out)
                gen_program(R, D, expertise, out_dir=out)
