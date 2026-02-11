import random
from collections import deque, defaultdict

def get_shortcuts(line_dim):
    shortcuts = set()
    for i in range(line_dim + 1):
        if random.random() > 0.8:
            random_k = random.choice([loc for loc in range(line_dim + 1) if loc != i])
            shortcuts.add((i, random_k))
    
    # to remove
    # shortcuts = set()
    return shortcuts

def build_graph(line_dim, shortcuts, bidir=True):
    G = defaultdict(set)

    # line edges
    for i in range(line_dim):
        u, v = i, i+1
        G[u].add(v)
        if bidir:
            G[v].add(u)

    # shortcut edges
    for (a,b) in shortcuts:
        if a <= line_dim and b <= line_dim and a != b:
            G[a].add(b)
            if bidir:
                G[b].add(a)

    return G

def bfs_path(G, start, goal):
    q = deque([start])
    parent = {start: None}

    while q:
        u = q.popleft()
        if u == goal:
            break
        for v in G[u]:
            if v not in parent:
                parent[v] = u
                q.append(v)

    if goal not in parent:
        return None  # unreachable
    # reconstruct
    path = []
    cur = goal
    while cur is not None:
        path.append(cur)
        cur = parent[cur]
    path.reverse()
    return path  # list of nodes

def moves_from_path(path):
    # path nodes are ints, 0 is kitchen l0, others are offices l1..ln
    moves = []
    for u, v in zip(path, path[1:]):
        moves.append(f"move_l{u}_l{v}")
    return moves

def create_instance(shortcuts, line_dim):
    prob_str = f"(define (problem robot_coffee_{line_dim})\n"
    prob_str += "(:domain robot_coffee)\n"
    prob_str += f"(:objects\n"
    prob_str += "\t" + " ".join([f"l{i}" for i in range(1, line_dim + 1)]) + " - room\n"
    prob_str += ")\n"
    prob_str += "(:init\n"
    prob_str += "\t(robot_at l0)\n"

    #G = build_graph(line_dim, shortcuts, bidir=True)
    #for u in range(line_dim + 1):
    #    for v in sorted(G[u]):
    #        prob_str += f"\t(connected l{u} l{v})\n"

    prob_str += ")\n"
    prob_str += f"(:goal (and " + " ".join([f"(coffee_at l{i})" for i in range(1, line_dim + 1)]) + ")))\n"
    return prob_str

def gen_problem_bw(shortcuts, line_dim):
    problem_str = create_instance(shortcuts, line_dim)
    with open(f"./benchmarks/robot_coffee_line/p{line_dim}.pddl", "w") as f:
        f.write(problem_str)

def write_program_exp1(shortcuts, line_dim):
    G = build_graph(line_dim, shortcuts, bidir=True)

    # all possible move actions that exist in THIS instance
    move_actions = []
    for u in range(0, line_dim+1):
        for v in sorted(G[u]):
            move_actions.append(f"move_l{u}_l{v}")
    move_somewhere = "(" + " | ".join(move_actions) + ")"

    deliver_coffee = "(" + " | ".join([f"put_coffee_l{i}" for i in range(1, line_dim+1)]) + ")"
    deliver_coffee_fair = "(" + " | ".join([f"put_coffee_fair_l{i}" for i in range(1, line_dim+1)]) + ")"

    pick_action = " | ".join([move_somewhere, deliver_coffee, deliver_coffee_fair])

    # allow prepare anytime, like you intended
    loop_body = f"(prepare_coffee_l0 | {pick_action})*"

    final_condition = "[" + " && ".join([f"coffee_at_l{i}" for i in range(1, line_dim+1)]) + "]?"
    return loop_body + " ; " + final_condition

def write_program_exp2(shortcuts, line_dim):
    G = build_graph(line_dim, shortcuts, bidir=True)

    deliveries = []
    for i in range(1, line_dim+1):
        p = bfs_path(G, 0, i)
        if p is None:
            continue
        go = " ; ".join(moves_from_path(p))
        back = " ; ".join(moves_from_path(list(reversed(p))))
        deliveries.append(
            f"(prepare_coffee_l0 ; {go} ; put_coffee_l{i} ; (nil | put_coffee_fair_l{i}) ; {back})"
        )

    body = "(" + " | ".join(deliveries) + ")*"
    final_condition = "[" + " && ".join([f"coffee_at_l{i}" for i in range(1, line_dim+1)]) + "]?"
    return body + " ; " + final_condition

def write_program_exp3(shortcuts, line_dim):
    G = build_graph(line_dim, shortcuts, bidir=True)

    parts = []
    for i in range(1, line_dim+1):
        p = bfs_path(G, 0, i)
        if p is None:
            continue  # or raise, depending on what you want
        go = " ; ".join(moves_from_path(p))
        back = " ; ".join(moves_from_path(list(reversed(p))))
        parts.append(
            f"(prepare_coffee_l0 ; {go} ; put_coffee_l{i} ; (nil | put_coffee_fair_l{i}) ; {back})"
        )

    body = "(" + " ; ".join(parts) + ")"
    final_condition = "[" + " && ".join([f"coffee_at_l{i}" for i in range(1, line_dim+1)]) + "]?"
    return body + " ; " + final_condition


def gen_program_bw(shortcuts, line_dim, expertise):
    if expertise == 1:
        program_str = write_program_exp1(shortcuts, line_dim)
    elif expertise == 2:
        program_str = write_program_exp2(shortcuts, line_dim)
    elif expertise == 3:
        program_str = write_program_exp3(shortcuts, line_dim)
    else:
        raise ValueError("Expertise level must be 1, 2, or 3.")
    
    with open(f"./benchmarks/robot_coffee_line/prog_{expertise}_{line_dim}.golog", "w") as f:
        f.write(program_str)


if __name__ == "__main__":
    shortcuts_all = get_shortcuts(100)
    for expertise in [1, 2, 3]:
        for line_dim in range(2, 30):
            shortcuts = list(filter(lambda shortcut: shortcut[0] <= line_dim and shortcut[1] <= line_dim, shortcuts_all))
            gen_problem_bw(shortcuts, line_dim)
            gen_program_bw(shortcuts, line_dim, expertise)