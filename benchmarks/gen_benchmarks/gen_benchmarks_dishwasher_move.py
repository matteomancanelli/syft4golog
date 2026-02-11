import os

def ensure_dir(path: str):
    os.makedirs(path, exist_ok=True)

# -----------------------
# DOMAIN GENERATION
# -----------------------
def create_domain(num_rooms: int) -> str:
    """
    Generate a DOMAIN file where actions are ground-friendly.
    Replaces forall/when with explicit parameters for dishes.
    """
    room_branches = "\n        ".join(
        [f"(and (dirty-dish-at ?d r{i}) (not (can-arrive ?d)))" for i in range(1, num_rooms + 1)]
    )

    dom = f"""(define (domain dishwasher)
  (:requirements :typing :strips :non-deterministic)
  (:types room dish)
  (:constants r0 - room)

  (:predicates
    (robot-at ?r - room)
    (can-arrive ?d - dish)
    (dirty-dish-at ?d - dish ?r - room)
    (clean ?d - dish)
    (holding ?d - dish)
    (emptyhand)
  )

  (:action move
    :parameters (?from - room ?to - room)
    :precondition (and (robot-at ?from))
    :effect (and (robot-at ?to) (not (robot-at ?from)))
  )

  (:action ask
    :parameters (?d - dish)
    :precondition (and (robot-at r0) (can-arrive ?d))
    :effect (oneof
        {room_branches}
      )
  )

  (:action load
    :parameters (?r - room ?d - dish)
    :precondition (and
      (robot-at ?r)
      (dirty-dish-at ?d ?r)
      (emptyhand)
    )
    :effect (and 
      (not (emptyhand)) 
      (not (dirty-dish-at ?d ?r)) 
      (holding ?d)
    )
  )

  (:action unload
    :parameters (?d - dish)
    :precondition (and
      (robot-at r0)
      (holding ?d)
    )
    :effect (and
      (emptyhand)
      (not (holding ?d))
      (clean ?d)
    )
  )
)
"""
    return dom

def domain_filename(num_rooms: int) -> str:
    return f"domain_r{num_rooms}.pddl"

# -----------------------
# PROBLEM GENERATION
# -----------------------
def create_problem(num_rooms: int, num_dishes: int) -> str:
    prob_name = f"dishwasher_{num_rooms}_{num_dishes}"
    rooms = " ".join([f"r{i}" for i in range(1, num_rooms + 1)])
    dishes = " ".join([f"d{i}" for i in range(1, num_dishes + 1)])

    prob = [
        f"(define (problem {prob_name})",
        "  (:domain dishwasher)",
        "  (:objects",
        f"    {dishes} - dish",
        f"    {rooms} - room",
        "  )",
        "  (:init",
        "    (robot-at r0)",
        "    (emptyhand)",
    ]
    for i in range(1, num_dishes + 1):
        prob.append(f"    (can-arrive d{i})")
    
    prob.append("  )")
    prob.append("  (:goal (and")
    for i in range(1, num_dishes + 1):
        prob.append(f"    (clean d{i})")
    prob.append("  ))")
    prob.append(")")
    return "\n".join(prob)

# -----------------------
# GOLOG PROGRAMS
# -----------------------
def get_action_sets(num_rooms, num_dishes):
    move_actions = []
    for r in range(1, num_rooms + 1):
        move_actions.append(f"move_r0_r{r}")
        move_actions.append(f"move_r{r}_r0")
    
    ask_actions = [f"ask_d{d}" for d in range(1, num_dishes + 1)]
    
    load_actions = []
    for r in range(1, num_rooms + 1):
        for d in range(1, num_dishes + 1):
            load_actions.append(f"load_r{r}_d{d}")
            
    unload_actions = [f"unload_d{d}" for d in range(1, num_dishes + 1)]
    
    return move_actions, ask_actions, load_actions, unload_actions

def write_program_exp1(num_rooms: int, num_dishes: int) -> str:
    moves, asks, loads, unloads = get_action_sets(num_rooms, num_dishes)
    all_acts = moves + asks + loads + unloads
    loop_body = f"(({' | '.join(all_acts)}) | nil)*"
    final = "[" + " && ".join([f"clean_d{i}" for i in range(1, num_dishes + 1)]) + "]?"
    return f"{loop_body} ; {final}"

def write_program_exp2(num_rooms: int, num_dishes: int) -> str:
    moves, asks, loads, unloads = get_action_sets(num_rooms, num_dishes)
    
    # Simple cycle: Ask -> Move -> Load -> Move Back -> Unload
    cycle = f"((nil | {' | '.join(asks)}) ; (nil | {' | '.join(moves)}) ; (nil | {' | '.join(loads)}) ; (nil | {' | '.join(moves)}) ; (nil | {' | '.join(unloads)}))"
    final = "[" + " && ".join([f"clean_d{i}" for i in range(1, num_dishes + 1)]) + "]?"
    return f"({cycle})* ; {final}"

def write_program_exp3(num_rooms: int, num_dishes: int) -> str:
    parts = []
    for d in range(1, num_dishes + 1):
        ask_d = f"ask_d{d}"
        move_to = f"({' | '.join([f'move_r0_r{r}' for r in range(1, num_rooms+1)])})"
        load_d = f"({' | '.join([f'load_r{r}_d{d}' for r in range(1, num_rooms+1)])})"
        move_back = f"({' | '.join([f'move_r{r}_r0' for r in range(1, num_rooms+1)])})"
        unload_d = f"unload_d{d}"
        parts.append(f"({ask_d} ; {move_to} ; {load_d} ; {move_back} ; {unload_d})")
    
    body = "(" + " ; ".join(parts) + ")"
    final = "[" + " && ".join([f"clean_d{i}" for i in range(1, num_dishes + 1)]) + "]?"
    return f"{body} ; {final}"

# -----------------------
# DRIVER
# -----------------------
def gen_instance(out_dir: str, num_rooms: int, num_dishes: int):
    ensure_dir(out_dir)
    with open(os.path.join(out_dir, domain_filename(num_rooms)), "w") as f:
        f.write(create_domain(num_rooms))
    with open(os.path.join(out_dir, f"p{num_rooms}_{num_dishes}.pddl"), "w") as f:
        f.write(create_problem(num_rooms, num_dishes))
    for exp in [1, 2, 3]:
        func = [write_program_exp1, write_program_exp2, write_program_exp3][exp-1]
        with open(os.path.join(out_dir, f"prog_{exp}_{num_rooms}_{num_dishes}.golog"), "w") as f:
            f.write(func(num_rooms, num_dishes))

if __name__ == "__main__":
    base = "./benchmarks/dishwasher_move"
    # Starting small for validation
    for r in range(1, 6):
        for d in range(1, 6):
            gen_instance(base, r, d)