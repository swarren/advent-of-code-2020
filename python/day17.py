#!/usr/bin/env python3

debug = False

with open("../input/day17.txt") as f:
    data = f.read().splitlines()

space = {}

y = -1
for l in data:
    y += 1
    for x, c in enumerate(l):
        if c == '#':
            space[(x, y, 0)] = c

def dump(space):
    xps = [coord[0] for coord in space.keys()]
    xmin = min(xps)
    xmax = max(xps)
    yps = [coord[1] for coord in space.keys()]
    ymin = min(yps)
    ymax = max(yps)
    zps = [coord[2] for coord in space.keys()]
    zmin = min(zps)
    zmax = max(zps)
    print(f'x {xmin}..{xmax} y {ymin}..{ymax} z {zmin}..{zmax}')
    print()
    for z in range(zmin, zmax + 1):
        print(f'z={z}')
        for y in range(ymin, ymax + 1):
            l = ''
            for x in range(xmin, xmax + 1):
                if (x, y, z) in space:
                    l += '#'
                else:
                    l += '.'
            print(l)
        print('')

if debug:
    print('Before any cycles:')
    print()
    dump(space)
for i in range(6):
    xps = [coord[0] for coord in space.keys()]
    xmin = min(xps)
    xmax = max(xps)
    yps = [coord[1] for coord in space.keys()]
    ymin = min(yps)
    ymax = max(yps)
    zps = [coord[2] for coord in space.keys()]
    zmin = min(zps)
    zmax = max(zps)

    newspace = {}
    for x in range(xmin - 1, xmax + 2):
        for y in range(ymin - 1, ymax + 2):
            for z in range(zmin - 1, zmax + 2):
                count = 0
                for xo in [-1, 0, 1]:
                    for yo in [-1, 0, 1]:
                        for zo in [-1, 0, 1]:
                            if xo == 0 and yo == 0 and zo == 0:
                                continue
                            if (x + xo, y + yo, z + zo) in space:
                                count += 1
                if (x, y, z) in space:
                    if count in (2, 3):
                        newspace[(x, y, z)] = '#'
                else:
                    if count == 3:
                        newspace[(x, y, z)] = '#'
    space = newspace
    if debug:
        if i == 0:
            s = ''
        else:
            s = 's'
        print('After ' + str(i + 1) + ' cycle' + s + ':')
        print()
        dump(space)
print(len(space))
