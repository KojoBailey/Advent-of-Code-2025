def turn_dial(direction, x, dx):
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

test_turn_dial()