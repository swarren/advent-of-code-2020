#!/usr/bin/env python3

import data11 as data

grid = data.data.splitlines()
#print(repr(grid))

def lookup(r, c, grid):
    if r < 0 or r >= len(grid):
        return 0
    if c < 0 or c >= len(grid[0]):
        return 0
    return grid[r][c]

dirs = tuple((dr, dc) for dr in range(-1, 2) for dc in range(-1, 2) if dr or dc)
def count_seen_filled(r, c, grid):
    #print('count_seen_filled', r, c, grid)
    count = 0
    for dr, dc in dirs:
        #print('  dir', dr, dc)
        cr = r
        cc = c
        while True:
            cr += dr
            cc += dc
            #print('    pos', cr, cc)
            if cr < 0 or cr >= len(grid):
                #print('      cr bounds')
                break
            if cc < 0 or cc >= len(grid[0]):
                #print('      cc bounds')
                break
            if grid[cr][cc] == '#':
                #print('      is #')
                count += 1
            if grid[cr][cc] != '.':
                #print('      not empty')
                break
    return count

def iter_grid(grid_in):
    ret = []
    for ir, r in enumerate(grid_in):
        outr = ''
        for ic, c in enumerate(r):
            if c == 'L':
                count = count_seen_filled(ir, ic, grid_in)
                oc = (count == 0) and '#' or 'L'
            elif c == '#':
                count = count_seen_filled(ir, ic, grid_in)
                oc = (count >= 5) and 'L' or '#'
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
