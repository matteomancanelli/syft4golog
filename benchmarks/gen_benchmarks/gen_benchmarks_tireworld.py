def get_adjacent_locations(i, j, grid_dim):
    adjacent = []
    if i > 1:
        adjacent.append((i - 1, j))
    if i < grid_dim:
        adjacent.append((i + 1, j))
    if j > 1:
        adjacent.append((i, j - 1))
    if j < grid_dim:
        adjacent.append((i, j + 1))
    if i > 1 and j > 1:
        adjacent.append((i - 1, j - 1))
    if i > 1 and j < grid_dim:
        adjacent.append((i - 1, j + 1))
    if i < grid_dim and j > 1:
        adjacent.append((i + 1, j - 1))
    if i < grid_dim and j < grid_dim:
        adjacent.append((i + 1, j + 1))
    
    adj_set = set()
    for (adj_i, adj_j) in adjacent:
        adj_set.add(f"l{i}{j} l{adj_i}{adj_j}")
    return adj_set

def create_instance(grid_dim):
    prob_str = f"(define (problem triangle_tire_{grid_dim})\n"
    prob_str += "(:domain triangle-tire)\n"
    prob_str += f"(:objects\n"
    prob_str += "\t" + " ".join([f"l{i}{j}" for i in range(1, grid_dim + 1) for j in range(1, grid_dim + 1)]) + " - location\n"
    prob_str += ")\n"
    prob_str += "(:init\n"
    prob_str += "\t(not-flattire)\n"
    prob_str += "\t(at l11)\n"

    adj_set = set()
    for subset in [get_adjacent_locations(i, j, grid_dim) for i in range(1, grid_dim + 1) for j in range(1, grid_dim + 1)]:
        adj_set = adj_set.union(subset)
    
    for elem in adj_set:
        prob_str += f"\t(connected {elem})\n"

    for i in range(1, grid_dim + 1):
        for j in range(1, grid_dim + 1):
            prob_str += f"\t(spare-in l{i}{j})\n"
    prob_str += ")\n"
    prob_str += f"(:goal (and (at l{grid_dim}{grid_dim}))))\n"
    return prob_str

def gen_problem_bw(grid_dim):
    problem_str = create_instance(grid_dim)
    with open(f"./benchmarks/tireworld/p{grid_dim}.pddl", "w") as f:
        f.write(problem_str)

def write_program_exp1(grid_dim):
    adj_set = set()
    for subset in [get_adjacent_locations(i, j, grid_dim) for i in range(1, grid_dim + 1) for j in range(1, grid_dim + 1)]:
        adj_set = adj_set.union(subset)
    
    move_somewhere = "("
    for elem in adj_set:
        move_somewhere += f"move_{elem.replace(' ', '_')} | "
    move_somewhere = move_somewhere[:-3] + ")"

    change_some_tire = "(" + "|".join([f"changetire_l{i}{j}" for i in range(1, grid_dim + 1) for j in range(1, grid_dim + 1)]) + ")"
    pick_action = move_somewhere + " | " + change_some_tire
    loop_body = "(" + pick_action + ")*"
    final_condition = f"[at_l{grid_dim}{grid_dim}]?"
    return loop_body + " ; " + final_condition

def write_program_exp2(grid_dim):
    move_and_change_tire = "|".join([f"(move_{elem.replace(' ', '_')} ; (nil | changetire_{elem.split(' ')[1]}))" for i in range(1, grid_dim + 1) for j in range(1, grid_dim + 1) for elem in get_adjacent_locations(i, j, grid_dim)])
    loop_body = "(" + move_and_change_tire + ")*"
    final_condition = f"[at_l{grid_dim}{grid_dim}]?"
    return loop_body + " ; " + final_condition

def write_program_exp3(grid_dim):
    body = "(" + " ; ".join([f"(move_l{i}{i}_l{i+1}{i+1} ; (nil | changetire_l{i+1}{i+1}))" for i in range(1, grid_dim + 1) if i != grid_dim]) + ")"
    final_condition = f"[at_l{grid_dim}{grid_dim}]?"
    return body + " ; " + final_condition

def gen_program_bw(grid_dim, expertise):
    if expertise == 1:
        program_str = write_program_exp1(grid_dim)
    elif expertise == 2:
        program_str = write_program_exp2(grid_dim)
    elif expertise == 3:
        program_str = write_program_exp3(grid_dim)
    else:
        raise ValueError("Expertise level must be 1, 2, or 3.")
    
    with open(f"./benchmarks/tireworld/prog_{expertise}_{grid_dim}.golog", "w") as f:
        f.write(program_str)


if __name__ == "__main__":
    for expertise in [1, 2, 3]:
        for grid_dim in range(2, 11):
            gen_problem_bw(grid_dim)
            gen_program_bw(grid_dim, expertise)