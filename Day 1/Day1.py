from typing import NamedTuple

def turn_dial(direction: str, x: int, dx: int) -> int:
    sign: int
    match direction:
        case 'L': sign = -1
        case 'R': sign = 1
    
    X_MAX = 100
    return (x + sign * dx) % X_MAX

def test_turn_dial():
    assert turn_dial('L', 50, 20) == 30
    assert turn_dial('L', 50, 80) == 70
    assert turn_dial('R', 50, 20) == 70
    assert turn_dial('R', 50, 80) == 30

class Instruction(NamedTuple):
    direction: str
    n: int

def evaluate_instructions(start: int, instructions: list[Instruction]) -> list[int]:
    result: list[int] = []
    buffer: int = start

    for instruction in instructions:
        buffer = turn_dial(instruction.direction, buffer, instruction.n)
        result.append(buffer)
    
    return result

def load_raw_instructions(path: str) -> list[str]:
    file = open(path)
    result: list[str] = []

    for line in file:
        result.append(line.replace("\n", ""))

    return result

def parse_instructions(raw_instructions: list[str]) -> list[Instruction]:
    result: list[Instruction] = []

    for raw_instruction in raw_instructions:
        result.append(Instruction(raw_instruction[0], int(raw_instruction[1:len(raw_instruction)])))
    
    return result

# main
test_turn_dial()

raw_instructions = load_raw_instructions("./Day 1/input.txt")
parsed_instructions = parse_instructions(raw_instructions)
print(parsed_instructions)

instructions = evaluate_instructions(50, [Instruction('R', 30), Instruction('R', 55), Instruction('L', 67)])
print(instructions)