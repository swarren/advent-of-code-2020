#!/usr/bin/env python3

import functools
import operator
import data20 as data

def s2i(s):
    v = 0
    for c in s:
        v <<= 1
        v |= ((c == '#') and 1 or 0)
    return v

tiles = {}
tile = None
ln = 0
ts = 0
ls = 0
rs = 0
for l in data.data.splitlines():
    ln += 1
    #print(ln, l)
    if 'Tile' in l:
        tile = int(l.split()[1][:-1])
        #print(f'  tile {tile}')
        ln = -1
        ls = ''
        rs = ''
        continue
    if 0 <= ln <= 9:
        ls = l[0] + ls
        rs = rs + l[-1]
    if ln == 0:
        ts = l
        t = s2i(ts)
        tr = s2i(ts[::-1])
    elif ln == 9:
        bs = l
        r = s2i(rs)
        rr = s2i(rs[::-1])
        b = s2i(bs[::-1])
        br = s2i(bs)
        lr = s2i(ls)
        l = s2i(ls[::-1])
        #print(tile, ts, rs, bs, ls)
        #print(tile, t, l, b, r)
        tiles[tile] = (t, tr, l, lr, b, br, r, rr)
#print(tiles)

# all_borders = [b for vs in tiles.values() for b in vs]

counts = {}
for tile, borders in tiles.items():
    for b in borders:
        counts[b] = counts.get(b, 0) + 1
#print(counts)

edge_tiles = []
corner_tiles = []
for tile, borders in tiles.items():
    edge_sides = []
    for i, border in enumerate(borders[::2]):
        #print(f'  {i} {border}')
        if counts[border] == 1:
            edge_sides.append(i)
    #print(tile, edge_sides)
    if edge_sides:
        edge_tiles.append(tile)
    if len(edge_sides) == 2:
        corner_tiles.append(tile)
#print(edge_tiles)
#print(corner_tiles)
print(functools.reduce(operator.mul, corner_tiles, 1))
