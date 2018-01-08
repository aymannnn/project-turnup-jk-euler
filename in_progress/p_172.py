'''
How many 18-digit numbers n (without leading zeros) are there such that
no digit occurs more than three times in n?
'''


def solve_problem():
    return


'''

Idiot, memory intensive, impossible BFS solution.

digits = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']

def is_possible(point, digit):
    return point.count(digit) < 3


def generate_next_points(starting_point):
    ending_points = []
    for point in starting_point:
        for digit in digits:
            if is_possible(point, digit):
                new_string = point + digit
                ending_points.append(new_string)
    return ending_points


def solve_problem():
    starting_point = ['1', '2', '3', '4', '5', '6', '7', '8', '9']
    digits_to_add = 6
    while digits_to_add > 0:
        print('Number of starting points ->', len(starting_point))
        starting_point = generate_next_points(starting_point)
        digits_to_add -= 1
    print(len(starting_point))


solve_problem()
'''