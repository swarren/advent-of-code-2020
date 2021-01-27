#!/usr/bin/env python3

with open("../input/day12.txt") as f:
    data = f.read()

waypoint = [10, 1]
pos = [0, 0]

def move_n(distance):
    waypoint[1] += int(distance)

def move_e(distance):
    waypoint[0] += int(distance)

def move_s(distance):
    waypoint[1] -= int(distance)

def move_w(distance):
    waypoint[0] -= int(distance)

rotations_l = {
    '90': 1j,
    '180': -1,
    '270': -1j,
}
def move_l(angle):
    global waypoint
    waypoint_c = waypoint[0] + 1j *  waypoint[1]
    waypoint_c *= rotations_l[angle]
    waypoint = [int(waypoint_c.real), int(waypoint_c.imag)]

rotations_r = {
    '90': -1j,
    '180': -1,
    '270': 1j,
}
def move_r(angle):
    global waypoint
    waypoint_c = waypoint[0] + 1j *  waypoint[1]
    waypoint_c *= rotations_r[angle]
    waypoint = [int(waypoint_c.real), int(waypoint_c.imag)]

def move_f(distance):
    pos[0] += waypoint[0] * int(distance)
    pos[1] += waypoint[1] * int(distance)

move_funcs = {
    'N': move_n,
    'E': move_e,
    'S': move_s,
    'W': move_w,
    'L': move_l,
    'R': move_r,
    'F': move_f,
}

#print(pos, waypoint)
for move in data.splitlines():
    #print(move)
    move_funcs[move[0]](move[1:])
    #print(pos, waypoint)

print(sum(map(abs, pos)))
