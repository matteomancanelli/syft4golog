import os
import random
from collections import defaultdict, deque

OUT_DIR = "./benchmarks/tireworld_simplified_ground"
N_MIN = 2
N_MAX = 30
SEED = 1
SHORTCUT_PROB = 0.0
SHORTCUT_EXTRA_EDGES_PER_NODE = 1
BIDIR = True

random.seed(SEED)

def ensure_dir(path):
    os.makedirs(path, exist_ok=True)

def build_edges(n):
    edges = set()
    for i in range(1, n):
        edges.add((i, i + 1))
        if BIDIR:
            edges.add((i + 1, i))
    for i in range(1, n + 1):
        for _ in range(SHORTCUT_EXTRA_EDGES_PER_NODE):
            if random.random() < SHORTCUT_PROB:
                j = random.choice([k for k in range(1, n + 1) if k != i])
                edges.add((i, j))
                if BIDIR:
                    edges.add((j, i))
    return edges

def build_graph(n, edges):
    G = defaultdict(set)
    for (u, v) in edges:
        G[u].add(v)
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
        return None
    path = []
    cur = goal
    while cur is not None:
        path.append(cur)
        cur = parent[cur]
    path.reverse()
    return path

def write_domain(n, edges):
    s = f"(define (domain triangle-tire-n{n})\n"
    s += "  (:requirements :typing :strips :non-deterministic)\n"
    s += "  (:types location)\n"
    s += "  (:predicates (at ?loc - location) (spare-in ?loc - location) (not-flattire) (can_move ?u ?v - location))\n"

    # SINGLE move action that takes two parameters
    s += """  (:action move
    :parameters (?u - location ?v - location)
    :precondition (and (at ?u) (not-flattire) (can_move ?u ?v))
    :effect (and (at ?v) (not (at ?u)) (oneof (and) (not (not-flattire))))
  )\n"""

    # SINGLE changetire action
    s += """  (:action changetire
    :parameters (?loc - location)
    :precondition (and (at ?loc) (spare-in ?loc))
    :effect (and (not (spare-in ?loc)) (not-flattire))
  )\n"""
    s += ")\n"
    return s, f"triangle-tire-n{n}"

def write_problem(n, edges, dom_name):
    s = f"(define (problem p{n})\n  (:domain {dom_name})\n"
    s += f"  (:objects " + " ".join([f"l{i}" for i in range(1, n+1)]) + " - location)\n"
    s += "  (:init (not-flattire) (at l1)\n"
    for u, v in edges:
        s += f"    (can_move l{u} l{v})\n"
    for i in range(1, n+1):
        s += f"    (spare-in l{i})\n"
    s += f"  )\n  (:goal (at l{n}))\n)"
    return s

def write_program_exp1(n, edges):
    move_actions = [f"move_l{u}_l{v}" for (u, v) in sorted(edges)]
    change_actions = [f"changetire_l{i}" for i in range(1, n + 1)]
    pick = "(" + " | ".join(move_actions + change_actions) + ")"
    return f"({pick})* ; [at_l{n}]?"

def write_program_exp2(n, edges):
    parts = []
    for (u, v) in sorted(edges):
        parts.append(f"((nil | move_l{u}_l{v}) ; (nil | changetire_l{v}))")
    body = "(" + " | ".join(parts) + ")*"
    return f"{body} ; [at_l{n}]?"

def write_program_exp3(n, edges):
    G = build_graph(n, edges)
    path = bfs_path(G, 1, n)
    if path is None or len(path) < 2:
        return f"(changetire_l1)* ; [at_l{n}]?"
    seq = []
    for u, v in zip(path, path[1:]):
        seq.append(f"(move_l{u}_l{v} ; (nil | changetire_l{v}))")
    body = "(" + " ; ".join(seq) + ")"
    return f"{body} ; [at_l{n}]?"

def main():
    ensure_dir(OUT_DIR)

    for n in range(N_MIN, N_MAX + 1):
        edges = build_edges(n)

        dom_str, dom_name = write_domain(n, edges)
        prob_str = write_problem(n, dom_name)

        prog1 = write_program_exp1(n, edges)
        prog2 = write_program_exp2(n, edges)
        prog3 = write_program_exp3(n, edges)

        with open(os.path.join(OUT_DIR, f"domain_n{n}.pddl"), "w") as f:
            f.write(dom_str)

        with open(os.path.join(OUT_DIR, f"p{n}.pddl"), "w") as f:
            f.write(prob_str)

        with open(os.path.join(OUT_DIR, f"prog_1_{n}.golog"), "w") as f:
            f.write(prog1)

        with open(os.path.join(OUT_DIR, f"prog_2_{n}.golog"), "w") as f:
            f.write(prog2)

        with open(os.path.join(OUT_DIR, f"prog_3_{n}.golog"), "w") as f:
            f.write(prog3)

if __name__ == "__main__":
    main()
