#!/usr/bin/env python3

import data12 as data

pos = [0, 0]
nesw = 'NESW'
pointing = 1
angles = {
    '90': 1,
    '180': 2,
    '270': 3,
}

def move_n(distance):
    pos[1] += int(distance)

def move_e(distance):
    pos[0] += int(distance)

def move_s(distance):
    pos[1] -= int(distance)

def move_w(distance):
    pos[0] -= int(distance)

def move_l(angle):
    global pointing
    delta = angles[angle]
    pointing = (pointing - delta) % len(nesw)

def move_r(angle):
    global pointing
    delta = angles[angle]
    pointing = (pointing + delta) % len(nesw)

def move_f(distance):
    global pointing
    move_funcs[nesw[pointing]](distance)

move_funcs = {
    'N': move_n,
    'E': move_e,
    'S': move_s,
    'W': move_w,
    'L': move_l,
    'R': move_r,
    'F': move_f,
}

#print(pos, nesw[pointing])
for move in data.data.splitlines():
    #print(move)
    move_funcs[move[0]](move[1:])
    #print(pos, nesw[pointing])

print(sum(map(abs, pos)))
