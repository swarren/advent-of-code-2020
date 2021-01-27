#!/usr/bin/env python3

import functools
import math

with open("../input/day20.txt") as f:
    data = f.read().splitlines()

def s2i(s):
    v = 0
    for c in s:
        v <<= 1
        v |= ((c == '#') and 1 or 0)
    return v

class Grid:
    def __init__(self, grid):
        self.grid = grid

    def grid_str(self, prefix=''):
        return '\n'.join(map(lambda x: prefix + x, self.grid))

    def rotated(self):
        sz = len(self.grid)
        new_grid = []
        for y in range(sz):
            new_grid.append(''.join(self.grid[sz-x-1][y] for x in range(sz)))
        return Grid(new_grid)

    def hflipped(self):
        sz = len(self.grid)
        new_grid = []
        for y in range(sz):
            new_grid.append(''.join(self.grid[y][sz-x-1] for x in range(sz)))
        return Grid(new_grid)

    def all_mutations(self):
        yield self
        yield self.hflipped()
        v = self
        for i in range(3):
            v = v.rotated()
            yield v
            yield v.hflipped()

class Tile(Grid):
    def __init__(self, tile_num, grid):
        super().__init__(grid)
        self.tile_num = tile_num
        self.edge_vals = self._calc_edge_vals()

    def __repr__(self):
        return f'Tile {self.tile_num}:\n' + self.grid_str('    ') + '\n    ' + repr(self.edge_vals)

    def _calc_edge_vals(self):
        ts = self.grid[0]
        bs = self.grid[-1]
        ls = ''.join(l[0] for l in self.grid) 
        rs = ''.join(l[-1] for l in self.grid) 
        return [
            s2i(ts),
            s2i(ts[::-1]),
            s2i(rs),
            s2i(rs[::-1]),
            s2i(bs),
            s2i(bs[::-1]),
            s2i(ls),
            s2i(ls[::-1]),
        ]

    def rotated(self):
        return Tile(self.tile_num, super().rotated().grid)

    def hflipped(self):
        return Tile(self.tile_num, super().hflipped().grid)

tiles = {}
tile_num = None
tile_grid = []
ln = 0
for l in data:
    ln += 1
    #print(ln, l)
    if 'Tile' in l:
        tile_num = int(l.split()[1][:-1])
        #print(f'  tile_num {tile_num}')
        tile_grid = []
        ln = -1
        continue
    if 0 <= ln <= 9:
        tile_grid.append(l)
    if ln == 9:
        tiles[tile_num] = Tile(tile_num, tile_grid)
#for tile in tiles.values():
#    print(tile)

counts = {}
for tile in tiles.values():
    for ev in tile.edge_vals:
        counts[ev] = counts.get(ev, 0) + 1
#print(counts)
outer_edge_vals = [ev for ev, count in counts.items() if count == 1]

edge_tiles = []
corner_tiles = []
for tile in tiles.values():
    edge_sides = []
    for i, border in enumerate(tile.edge_vals[::2]):
        #print(f'  {i} {border}')
        if border in outer_edge_vals:
            edge_sides.append(i)
    #print(tile, edge_sides)
    if edge_sides:
        edge_tiles.append(tile)
    if len(edge_sides) == 2:
        corner_tiles.append(tile)
#print('edge tiles:', map(lambda t: t.tile_num, edge_tiles))
#print('corner tiles:', map(lambda t: t.tile_num, corner_tiles))

#t0 = next(iter(tiles.values()))
#print('0', t0)
#t90 = t0.rotated()
#print('90', t90)
#t180 = t90.rotated()
#print('180', t180)
#t270 = t180.rotated()
#print('270', t270)
#t360 = t270.rotated()
#print('360', t360)
#t0h = t0.hflipped()
#print('h', t0h)
#t0hh = t0h.hflipped()
#print('hh', t0hh)

#t = next(iter(tiles.values()))
#for tm in t.all_mutations():
#    print(tm)

def mutate_to_constraints(tile, constraints):
    for tm in tile.all_mutations():
        if tm.edge_vals[0] not in constraints[0]:
            continue
        if tm.edge_vals[6] not in constraints[1]:
            continue
        return tm
    return None

tile_board = {}
tile_board_sz = int(math.sqrt(len(tiles)))
for y in range(tile_board_sz):
    for x in range(tile_board_sz):
        if x == 0:
            if y == 0:
                top_left_tile = corner_tiles[0]
                constraints = [outer_edge_vals, outer_edge_vals]
                tile = mutate_to_constraints(top_left_tile, constraints)
            else:
                constraints = [[tile_board[y - 1, x].edge_vals[4]], outer_edge_vals]
                tile = None
        else:
            if y == 0:
                constraints = [outer_edge_vals, [tile_board[y, x - 1].edge_vals[2]]]
            else:
                constraints = [[tile_board[y - 1, x].edge_vals[4]], [tile_board[y, x - 1].edge_vals[2]]]
            tile = None
        if not tile:
            for tile in tiles.values():
                tile = mutate_to_constraints(tile, constraints)
                if tile:
                    break
        tile_board[y, x] = tile
        del tiles[tile.tile_num]

#for y in range(tile_board_sz):
#    print(', '.join(str(tile_board[y, x].tile_num) for x in range(tile_board_sz)))

board_grid = []
for y in range(tile_board_sz):
    for ty in range(10 - 2):
        board_grid.append(''.join(tile_board[y, x].grid[ty + 1][1:-1] for x in range(tile_board_sz)))
board = Grid(board_grid)
board_sz = len(board_grid[0])

monster_s = [
    '                  # ',
    '#    ##    ##    ###',
    ' #  #  #  #  #  #   '
]
monster = list(map(s2i, monster_s))
monster_width = len(monster_s[0])

def count_monsters(board):
    count = 0
    igrid = list(map(s2i, board.grid))
    for y in range(len(igrid) - 2):
        for x in range(board_sz - monster_width + 1):
            match = True
            for yo in range(len(monster)):
                mask = monster[yo] << (board_sz - monster_width - x)
                if igrid[y + yo] & mask != mask:
                    match = False
                    break
            if match:
                #print(y + yo, x)
                count += 1
    return count

def count_hashes(board):
    return sum(map(lambda c: c == '#' and 1 or 0, ''.join(board.grid)))

for board_mutated in board.all_mutations():
    #print('board_mutated:')
    #print(board_mutated.grid_str('    ')) 
    monster_count = count_monsters(board_mutated)
    #print('monster_count', monster_count)
    if not monster_count:
        continue
    roughness = count_hashes(board_mutated) - (15 * monster_count)
    break
print(roughness)
