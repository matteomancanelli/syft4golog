def create_instance(num_shelves, num_boxes):
    prob_str = f"(define (problem warehouse_{num_shelves}_{num_boxes})\n"
    prob_str += "(:domain warehouse-domain)\n"
    prob_str += f"(:objects\n"
    prob_str += "\t" + " ".join([f"b{i}" for i in range(1, num_boxes + 1)]) + " - box\n"
    prob_str += "\t" + " ".join([f"s{i}" for i in range(1, num_shelves + 1)]) + " - shelf\n"
    prob_str += ")\n"
    prob_str += "(:init\n"
    prob_str += "\t(emptyhand)\n"
    prob_str += "\t(robot-at s0)\n"
    for i in range(1, num_shelves + 1):
        prob_str += f"\t(box-at b{i} s{i})\n"
    prob_str += ")\n"
    prob_str += "(:goal (and\n"
    for i in range(1, num_boxes+1):
        prob_str += f"\t(box-at b{i} s0)\n"
    prob_str += ")))\n"
    return prob_str

def gen_problem_bw(num_shelves, num_boxes):
    problem_str = create_instance(num_shelves, num_boxes)
    with open(f"./benchmarks/warehouse/p{num_shelves}_{num_boxes}.pddl", "w") as f:
        f.write(problem_str)

def write_program_exp1(num_shelves, num_blocks):
    move_somewhere = "(" + "|".join([f"move_s0_s{i} | move_s{i}_s0" for i in range(1, num_shelves+1)]) + ")"
    take_some_box = "(" + "|".join([f"take_b{i}_s{i}" for i in range(1, num_blocks + 1)]) + ")"
    drop_some_box = "(" + "|".join([f"drop_b{i}_s0" for i in range(1, num_blocks + 1)]) + ")"
    wrap_some_box = "(" + "|".join([f"wrap_b{i}" for i in range(1, num_blocks + 1)]) + ")"
    drop_wrapped_box = "(" + "|".join([f"drop_wrap_b{i}_s0" for i in range(1, num_blocks + 1)]) + ")"
    pick_action = move_somewhere + " | " + take_some_box + " | " + drop_some_box + " | " + wrap_some_box + " | " + drop_wrapped_box
    loop_body = "(" + pick_action + ")*"
    final_condition = "[" + " && ".join([f"box_at_b{i}_s0" for i in range(1, num_blocks + 1)]) + "]?"
    return loop_body + " ; " + final_condition

def write_program_exp2(num_shelves, num_blocks):
    deliver_some_block = "|".join([f"(move_s0_s{i} ; take_b{i}_s{i} ; move_s{i}_s0 ; (drop_b{i}_s0 | (wrap_b{i} ; drop_wrap_b{i}_s0)))" for i in range(1, num_shelves + 1)])
    loop_body = "(" + deliver_some_block + ")*"
    final_condition = "[" + " && ".join([f"box_at_b{i}_s0" for i in range(1, num_blocks + 1)]) + "]?"
    return loop_body + " ; " + final_condition

def write_program_exp3(num_shelves, num_blocks):
    body = "(" + " ; ".join([f"(move_s0_s{i} ; take_b{i}_s{i} ; move_s{i}_s0 ; wrap_b{i} ; drop_wrap_b{i}_s0)" for i in range(1, num_shelves + 1)]) + ")"
    final_condition = "[" + " && ".join([f"box_at_b{i}_s0" for i in range(1, num_blocks + 1)]) + "]?"
    return body + " ; " + final_condition

def gen_program_bw(num_shelves, num_blocks, expertise):
    if expertise == 1:
        program_str = write_program_exp1(num_shelves, num_blocks)
    elif expertise == 2:
        program_str = write_program_exp2(num_shelves, num_blocks)
    elif expertise == 3:
        program_str = write_program_exp3(num_shelves, num_blocks)
    else:
        raise ValueError("Expertise level must be 1, 2, or 3.")
    
    with open(f"./benchmarks/warehouse/prog_{expertise}_{num_shelves}_{num_blocks}.golog", "w") as f:
        f.write(program_str)


if __name__ == "__main__":
    for expertise in [1, 2, 3]:
        for num_shelves in range(2, 11):
            gen_problem_bw(num_shelves, num_shelves)
            gen_program_bw(num_shelves, num_shelves, expertise)