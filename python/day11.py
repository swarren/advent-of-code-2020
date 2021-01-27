#!/usr/bin/env python3

with open("../input/day11.txt") as f:
    data = f.read()

grid = data.splitlines()
#print(repr(grid))

def lookup(r, c, grid):
    if r < 0 or r >= len(grid):
        return 0
    if c < 0 or c >= len(grid[0]):
        return 0
    return grid[r][c]

def count_adjacent_filled(r, c, grid):
    return sum([(lookup(irx, icx, grid) == '#') and 1 or 0 for irx in range(r - 1, r + 2) for icx in range(c - 1, c + 2) if not ((irx == r) and (icx == c))])

def iter_grid(grid_in):
    ret = []
    for ir, r in enumerate(grid_in):
        outr = ''
        for ic, c in enumerate(r):
            if c == 'L':
                count = count_adjacent_filled(ir, ic, grid_in)
                oc = (count == 0) and '#' or 'L'
            elif c == '#':
                count = count_adjacent_filled(ir, ic, grid_in)
                oc = (count >= 4) and 'L' or '#'
            else:
                oc = '.'
            outr = outr + oc
        ret.append(outr)
    return ret

while True:
    ngrid = iter_grid(grid)
    #print(repr(ngrid))
    if ngrid == grid:
        break
    grid = ngrid

print(len(list(filter(lambda c: c == '#', ''.join(grid)))))
