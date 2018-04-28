#!/bin/python3
'''
ellipse with 4x^2 + y^2 = 100 (p144)
-0.01 <= x <= 0.01 is missing, allowing light into or out of exit
xo = (0.0, 10.1) -> (1.4, -9.6)
law of reflection, slope of tangent line is -4x / y
normal line is perpendicular to tangent
how many times does it bounce inside before exiting?
'''

# y = mx + b
# y - mx = b

import math


def get_y_int(p, m):
    return p[1] - m * p[0]


def get_intersection_point(p1, m1, p2, m2):
    x1 = p1[0]
    y1 = p1[1]
    x2 = p2[0]
    y2 = p2[1]
    x = (-m1 * x1 + m2 * x2 - y2 + y1) / (m2 - m1)
    y = m2 * (x - x2) + y2
    return (x, y)


def line_intersect_circle_point(p_on_already, m, b):
    # Ellipse is defined as
    # 4x^2 + y^2 = 100
    # intersection with y = mx + b
    # 4x^2 + (mx + b)^2 = 100
    # 4x^2 + m^2x^2 + 2mbx + b^2 = 100
    # (4 + m^2)x^2 + 2mbx + (b^2 - 100) = 0
    qa = 4 + (m * m)
    qb = 2 * m * b
    qc = (b * b) - 100
    x1 = (-qb + math.sqrt(qb * qb - 4 * qa * qc)) / (2 * qa)
    x2 = (-qb - math.sqrt(qb * qb - 4 * qa * qc)) / (2 * qa)
    intersect_x = None
    if math.isclose(x1, p_on_already[0]):
        intersect_x = x2
    elif math.isclose(x2, p_on_already[0]):
        intersect_x = x1
    else:
        print('Error')
    # Now we just need the y value, which we can get by plugging into
    # the original y = mx + b
    intersect_y = m * intersect_x + b
    p5 = (intersect_x, intersect_y)
    return p5


def get_distance(p1, p2):
    return math.sqrt((p2[0] - p1[0])**2 + (p2[1] - p1[1])**2)


def is_point_on_line(point, slope_of_line, intercept):
    y = slope_of_line * point[0] + intercept
    return y == point[1]


def get_slope(p1, p2):
    return (p2[1] - p1[1]) / (p2[0] - p1[0])


def get_next_circle_intersect(p2, p1):
    # Describe previous line, we know p2 is on circle
    slope_tangent = (-4 * p2[0]) / p2[1]
    slope_normal = -1 / slope_tangent
    # Let p3 be the intersection point between the normal line and
    # line from p1 with a slope of the tangent at p2
    p3 = get_intersection_point(p1, slope_tangent, p2, slope_normal)
    change_y = p1[1] - p3[1]
    change_x = p1[0] - p3[0]
    # Let p4 be the line from p1 to p3 continued an equal distance so
    # we get the right angle
    p4 = (p3[0] - change_x, p3[1] - change_y)
    # Now we just need to see when line p2 -> p4 intersects with circle
    slope_p2_p4 = get_slope(p2, p4)
    y_int_p2_p4 = get_y_int(p2, slope_p2_p4)
    p5 = line_intersect_circle_point(p2, slope_p2_p4, y_int_p2_p4)
    # p5 is the point on the circle
    return p5


def solve(destination, initial_point):
    current_p2 = destination
    current_p1 = initial_point
    next_p2 = None
    count = 1
    # history = []
    while True:
        next_p2 = get_next_circle_intersect(current_p2, current_p1)
        if next_p2[0] < 0.01 and next_p2[0] > -0.01 and next_p2[1] > 5:
            break
        else:
            count += 1
            # history.append(next_p2)
        current_p1 = current_p2
        current_p2 = next_p2
    #return count, history
    return count


if __name__ == '__main__':
    initial_point = (0.0, 10.1)
    destination = (1.4, -9.6)
    solution = solve(destination, initial_point)
    print('Total Count :', solution)
