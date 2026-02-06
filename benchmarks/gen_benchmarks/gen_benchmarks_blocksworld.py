def create_tower(num_blocks):
    prob_str = f"(define (problem bw_{num_blocks})\n"
    prob_str += "(:domain blocks-table)\n"
    prob_str += f"(:objects " + " ".join([f"b{i}" for i in range(1, num_blocks + 1)]) + " - block)\n"
    prob_str += "(:init\n"
    prob_str += "\t(emptyhand)\n"
    for i in range(1, num_blocks):
        prob_str += f"\t(on b{i} b{i + 1})\n"
    prob_str += f"\t(on b{num_blocks} b0)\n"
    prob_str += "\t(clear b1)\n"    
    prob_str += ")\n"
    prob_str += "(:goal (and\n"
    for i in range(1, num_blocks+1):
        prob_str += f"\t(on b{i} b0)\n"
    prob_str += ")))\n"
    return prob_str

def gen_problem_bw(num_blocks):
    problem_str = create_tower(num_blocks)
    with open(f"./benchmarks/blocksworld/p{num_blocks}.pddl", "w") as f:
        f.write(problem_str)

def write_program_exp1(num_blocks):
    unstack_some_block = "(" + "|".join([f"unstack_b{i}_b{i+1}" for i in range(1, num_blocks)]) + ")"
    put_down_some_block = "(" + "|".join([f"put_down_b{i}" for i in range(1, num_blocks + 1)]) + ")"
    pick_action = unstack_some_block + " | " + put_down_some_block
    loop_body = "(" + pick_action + ")*"
    final_condition = "[" + " && ".join([f"on_b{i}_b0" for i in range(1, num_blocks + 1)]) + "]?"
    return loop_body + " ; " + final_condition

def write_program_exp2(num_blocks):
    unstack_some_block = "(" + "|".join([f"unstack_b{i}_b{i+1}" for i in range(1, num_blocks)]) + ")"
    put_down_some_block = "(" + "|".join([f"put_down_b{i}" for i in range(1, num_blocks + 1)]) + ")"
    loop_body = "((nil | " + unstack_some_block + ") ; (nil | " + put_down_some_block + "))*"
    final_condition = "[" + " && ".join([f"on_b{i}_b0" for i in range(1, num_blocks + 1)]) + "]?"
    return loop_body + " ; " + final_condition

def write_program_exp3(num_blocks):
    body = "(" + " ; ".join([f"(unstack_b{i}_b{i+1} ; (nil | put_down_b{i}))" for i in range(1, num_blocks)]) + ")"
    final_condition = "[" + " && ".join([f"on_b{i}_b0" for i in range(1, num_blocks + 1)]) + "]?"
    return body + " ; " + final_condition

def gen_program_bw(num_blocks, expertise):
    if expertise == 1:
        program_str = write_program_exp1(num_blocks)
    elif expertise == 2:
        program_str = write_program_exp2(num_blocks)
    elif expertise == 3:
        program_str = write_program_exp3(num_blocks)
    else:
        raise ValueError("Expertise level must be 1, 2, or 3.")
    
    with open(f"./benchmarks/blocksworld/prog_{expertise}_{num_blocks}.golog", "w") as f:
        f.write(program_str)

if __name__ == "__main__":
    for expertise in [1, 2, 3]:
        for num_blocks in range(3, 21):
            gen_problem_bw(num_blocks)
            gen_program_bw(num_blocks, expertise)