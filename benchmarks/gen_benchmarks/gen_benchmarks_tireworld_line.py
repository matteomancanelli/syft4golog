import random

def get_shortcuts(line_dim):
    shortcuts = set()
    for i in range(1, line_dim + 1):
        if i > 1 and random.random() > 0.8:
            random_k = random.choice([loc for loc in range(1, line_dim+1) if loc != i])
            shortcuts.add((i, random_k))
    
    # to remove
    shortcuts = set()
    return shortcuts

def get_adjacent_locations(i, shortcuts, line_dim):
    adjacent = []
    if i > 1:
        adjacent.append(i - 1)
    if i < line_dim:
        adjacent.append(i + 1)
    
    adj_set = set()
    for adj_i in adjacent:
        adj_set.add(f"l{i} l{adj_i}")
    
    for shortcut in shortcuts:
        if shortcut[0] == i:
            adj_set.add(f"l{i} l{shortcut[1]}")
        elif shortcut[1] == i:
            adj_set.add(f"l{i} l{shortcut[0]}")
    
    return adj_set

def create_instance(shortcuts, line_dim):
    prob_str = f"(define (problem triangle_tire_{line_dim})\n"
    prob_str += "(:domain triangle-tire)\n"
    prob_str += f"(:objects\n"
    prob_str += "\t" + " ".join([f"l{i}" for i in range(1, line_dim + 1)]) + " - location\n"
    prob_str += ")\n"
    prob_str += "(:init\n"
    prob_str += "\t(not-flattire)\n"
    prob_str += "\t(at l1)\n"

    adj_set = set()
    for subset in [get_adjacent_locations(i, shortcuts, line_dim) for i in range(1, line_dim + 1)]:
        adj_set = adj_set.union(subset)
    
    for elem in adj_set:
        prob_str += f"\t(connected {elem})\n"

    for i in range(1, line_dim + 1):
        prob_str += f"\t(spare-in l{i})\n"
    prob_str += ")\n"
    prob_str += f"(:goal (and (at l{line_dim}))))\n"
    return prob_str

def gen_problem_bw(shortcuts, line_dim):
    problem_str = create_instance(shortcuts, line_dim)
    with open(f"./benchmarks/tireworld_line/p{line_dim}.pddl", "w") as f:
        f.write(problem_str)

def write_program_exp1(shortcuts, line_dim):
    adj_set = set()
    for subset in [get_adjacent_locations(i, shortcuts, line_dim) for i in range(1, line_dim + 1)]:
        adj_set = adj_set.union(subset)
    
    move_somewhere = "("
    for elem in adj_set:
        move_somewhere += f"move_{elem.replace(' ', '_')} | "
    move_somewhere = move_somewhere[:-3] + ")"

    change_some_tire = "(" + "|".join([f"changetire_l{i}" for i in range(1, line_dim + 1)]) + ")"
    pick_action = move_somewhere + " | " + change_some_tire
    loop_body = "(" + pick_action + ")*"
    final_condition = f"[at_l{line_dim}]?"
    return loop_body + " ; " + final_condition

def write_program_exp2(shortcuts, line_dim):
    move_and_change_tire = "|".join([f"((nil | move_{elem.replace(' ', '_')}) ; (nil | changetire_{elem.split(' ')[1]}))" for i in range(1, line_dim + 1) for elem in get_adjacent_locations(i, shortcuts, line_dim)])
    loop_body = "(" + move_and_change_tire + ")*"
    final_condition = f"[at_l{line_dim}]?"
    return loop_body + " ; " + final_condition

def write_program_exp3(line_dim):
    body = "(" + " ; ".join([f"(move_l{i}_l{i+1} ; (nil | changetire_l{i+1}))" for i in range(1, line_dim + 1) if i != line_dim]) + ")"
    final_condition = f"[at_l{line_dim}]?"
    return body + " ; " + final_condition

def gen_program_bw(shortcuts, line_dim, expertise):
    if expertise == 1:
        program_str = write_program_exp1(shortcuts, line_dim)
    elif expertise == 2:
        program_str = write_program_exp2(shortcuts, line_dim)
    elif expertise == 3:
        program_str = write_program_exp3(line_dim)
    else:
        raise ValueError("Expertise level must be 1, 2, or 3.")
    
    with open(f"./benchmarks/tireworld_line/prog_{expertise}_{line_dim}.golog", "w") as f:
        f.write(program_str)


if __name__ == "__main__":
    for expertise in [1, 2, 3]:
        for line_dim in range(2, 30):
            shortcuts = get_shortcuts(line_dim)
            gen_problem_bw(shortcuts, line_dim)
            gen_program_bw(shortcuts, line_dim, expertise)