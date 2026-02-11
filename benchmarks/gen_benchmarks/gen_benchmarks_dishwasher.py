import os

def ensure_dir(path: str):
    os.makedirs(path, exist_ok=True)

# -----------------------
# DOMAIN GENERATION
# -----------------------
def create_domain(num_rooms: int) -> str:
    room_branches = "\n        ".join(
        [f"(and (dirty-dish-at ?d r{i}) (not (can-arrive ?d)))" for i in range(1, num_rooms + 1)]
    )

    dom = f"""(define (domain dishwasher)
  (:requirements :typing :strips :non-deterministic)
  (:types room dish)

  (:predicates
    (can-arrive ?d - dish)
    (dirty-dish-at ?d - dish ?r - room)
    (clean ?d - dish)
    (holding ?d - dish)
  )

  (:action ask
    :parameters (?d - dish)
    :precondition (and (can-arrive ?d))
    :effect (oneof
        {room_branches}
      )
  )

  (:action load
    :parameters (?r - room ?d - dish)
    :precondition (and (dirty-dish-at ?d ?r))
    :effect (and 
      (not (dirty-dish-at ?d ?r)) 
      (holding ?d)
    )
  )

  (:action unload
    :parameters (?d - dish)
    :precondition (and (holding ?d))
    :effect (and
      (not (holding ?d))
      (clean ?d)
    )
  )
)
"""
    return dom

# -----------------------
# GOLOG PROGRAMS
# -----------------------
def get_action_sets(num_rooms, num_dishes):
    asks = [f"ask_d{d}" for d in range(1, num_dishes + 1)]
    
    loads = []
    for r in range(1, num_rooms + 1):
        for d in range(1, num_dishes + 1):
            loads.append(f"load_r{r}_d{d}")
            
    unloads = [f"unload_d{d}" for d in range(1, num_dishes + 1)]
    
    return asks, loads, unloads

def write_program_exp1(num_rooms: int, num_dishes: int) -> str:
    asks, loads, unloads = get_action_sets(num_rooms, num_dishes)
    all_acts = asks + loads + unloads
    loop_body = f"(({' | '.join(all_acts)}) | nil)*"
    final = "[" + " && ".join([f"clean_d{i}" for i in range(1, num_dishes + 1)]) + "]?"
    return f"{loop_body} ; {final}"

def write_program_exp2(num_rooms: int, num_dishes: int) -> str:
    asks, loads, unloads = get_action_sets(num_rooms, num_dishes)
    # Reactive cycle: Any ask -> Any load -> Any unload
    cycle = f"(({' | '.join(asks)}) ; ({' | '.join(loads)}) ; ({' | '.join(unloads)}))"
    final = "[" + " && ".join([f"clean_d{i}" for i in range(1, num_dishes + 1)]) + "]?"
    return f"({cycle} | nil)* ; {final}"

def write_program_exp3(num_rooms: int, num_dishes: int) -> str:
    # Most restrictive: Sequential dish processing
    parts = []
    for d in range(1, num_dishes + 1):
        ask_d = f"ask_d{d}"
        # Agent must be able to load from ANY room the environment chose
        load_d = f"({' | '.join([f'load_r{r}_d{d}' for r in range(1, num_rooms+1)])})"
        unload_d = f"unload_d{d}"
        parts.append(f"({ask_d} ; {load_d} ; {unload_d})")
    
    body = "(" + " ; ".join(parts) + ")"
    final = "[" + " && ".join([f"clean_d{i}" for i in range(1, num_dishes + 1)]) + "]?"
    return f"{body} ; {final}"

# -----------------------
# DRIVER
# -----------------------
def gen_instance(out_dir: str, num_rooms: int, num_dishes: int):
    ensure_dir(out_dir)
    with open(os.path.join(out_dir, f"domain_r{num_rooms}.pddl"), "w") as f:
        f.write(create_domain(num_rooms))
    
    # Simple problem generator logic
    dishes = " ".join([f"d{i}" for i in range(1, num_dishes + 1)])
    rooms = " ".join([f"r{i}" for i in range(1, num_rooms + 1)])
    init_str = "\n    ".join([f"(can-arrive d{i})" for i in range(1, num_dishes + 1)])
    goal_str = "\n    ".join([f"(clean d{i})" for i in range(1, num_dishes + 1)])
    
    prob = f"""(define (problem p_{num_rooms}_{num_dishes})
  (:domain dishwasher)
  (:objects {dishes} - dish {rooms} - room)
  (:init {init_str})
  (:goal (and {goal_str}))
)"""
    with open(os.path.join(out_dir, f"p{num_rooms}_{num_dishes}.pddl"), "w") as f:
        f.write(prob)

    for exp in [1, 2, 3]:
        func = [write_program_exp1, write_program_exp2, write_program_exp3][exp-1]
        with open(os.path.join(out_dir, f"prog_{exp}_{num_rooms}_{num_dishes}.golog"), "w") as f:
            f.write(func(num_rooms, num_dishes))

if __name__ == "__main__":
    base = "./benchmarks/dishwasher"
    for r in range(1, 6):
        for d in range(1, 6):
            gen_instance(base, r, d)