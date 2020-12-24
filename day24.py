#!/usr/bin/env python3

import data24 as data

# e, se, sw, w, nw, and ne

black = {}
for l in data.data.splitlines():
    #print(l)
    pos = [0, 0]
    li = 0
    while li < len(l):
        if l[li] == 'e':
            pos[0] += 1
            li += 1
        elif l[li] == 'w':
            pos[0] -= 1
            li += 1
        elif l[li] == 'n':
            if l[li + 1] == 'e':
                pos[0] += 0.5
            else:
                pos[0] -= 0.5
            pos[1] += 0.5
            li += 2
        else:
            if l[li + 1] == 'e':
                pos[0] += 0.5
            else:
                pos[0] -= 0.5
            pos[1] -= 0.5
            li += 2
        #print(' ', pos)
    pos = tuple(pos)
    if pos in black:
        del black[pos]
    else:
        black[pos] = 1
print(len(black))
