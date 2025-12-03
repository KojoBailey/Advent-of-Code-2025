from typing import NamedTuple
import math

class Instruction(NamedTuple):
    direction: str
    n: int

class DialTurn(NamedTuple):
    n: int
    click_count: int

def turn_dial(direction: str, x: int, dx: int) -> DialTurn:
    sign: int
    match direction:
        case 'L': sign = -1
        case 'R': sign = 1
    
    subtraction: int = x + sign * dx
    X_MAX: int = 100
    clamp: int = subtraction % X_MAX
    click_count: int = abs(math.floor(subtraction / 100))
    return DialTurn(clamp, click_count)

def test_turn_dial():
    assert turn_dial('L', 50, 20) == DialTurn(30, 0) 
    assert turn_dial('L', 50, 80) == DialTurn(70, 1) 
    assert turn_dial('R', 50, 20) == DialTurn(70, 0) 
    assert turn_dial('R', 50, 80) == DialTurn(30, 1) 
    assert turn_dial('R', 50, 50) == DialTurn(0, 1) 
    assert turn_dial('R', 5, 500) == DialTurn(5, 5) 

def evaluate_instructions(start: int, instructions: list[Instruction]) -> list[DialTurn]:
    result: list[DialTurn] = []
    buffer: int = start
    buffer2: DialTurn

    for instruction in instructions:
        buffer2 = turn_dial(instruction.direction, buffer, instruction.n)
        buffer = buffer2.n
        result.append(buffer2)
    
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

def calculate_password(path: str) -> int:
    DIAL_START: int = 50
    raw_instructions: list[str] = load_raw_instructions(path)
    parsed_instructions: list[Instruction] = parse_instructions(raw_instructions)
    instructions: list[DialTurn] = evaluate_instructions(DIAL_START, parsed_instructions)

    acc: int = 0
    for instruction in instructions:
        if instruction.n == 0: acc += 1

    return acc

def calculate_click_password(path: str) -> int:
    DIAL_START: int = 50
    raw_instructions: list[str] = load_raw_instructions(path)
    parsed_instructions: list[Instruction] = parse_instructions(raw_instructions)
    instructions: list[DialTurn] = evaluate_instructions(DIAL_START, parsed_instructions)

    acc: int = 0
    for instruction in instructions:
        acc += instruction.click_count

    return acc


# main
test_turn_dial()

print(calculate_password("./Day 1/input.txt"))
print(calculate_click_password("./Day 1/input.txt"))