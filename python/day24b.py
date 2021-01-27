#!/usr/bin/env python3

with open("../input/day24.txt") as f:
    data = f.read().splitlines()

# e, se, sw, w, nw, and ne

black = {}
for l in data:
    #print(l)
    pos = [0, 0]
    li = 0
    while li < len(l):
        if l[li] == 'e':
            pos[0] += 2
            li += 1
        elif l[li] == 'w':
            pos[0] -= 2
            li += 1
        elif l[li] == 'n':
            if l[li + 1] == 'e':
                pos[0] += 1
            else:
                pos[0] -= 1
            pos[1] += 1
            li += 2
        else:
            if l[li + 1] == 'e':
                pos[0] += 1
            else:
                pos[0] -= 1
            pos[1] -= 1
            li += 2
        #print(' ', pos)
    pos = tuple(pos)
    if pos in black:
        del black[pos]
    else:
        black[pos] = 1

offsets = (
    (-2, 0),
    (2, 0),
    (-1, -1),
    (-1, 1),
    (1, -1),
    (1, 1),
)

for day in range(1, 101):
    #print('day', day)
    black_pos = list(black.keys())
    #print(black_pos)
    ew_pos = list(map(lambda pos: pos[0], black_pos))
    #print(list(ew_pos))
    ns_pos = list(map(lambda pos: pos[1], black_pos))
    #print(list(ns_pos))
    ew_min = min(ew_pos) - 3
    ew_max = max(ew_pos) + 3
    ns_min = min(ns_pos) - 3
    ns_max = max(ns_pos) + 3

    new_black = {}
    for ns in range(ns_min, ns_max):
        for ew in range(ew_min, ew_max):
            if ns % 2:
                ew -= 1
            pos = (ew, ns)
            #print('  pos', pos)
            count = 0
            for offset in offsets:
                test_pos = (pos[0] + offset[0], pos[1] + offset[1])
              #  print('    test_pos', test_pos)
                if test_pos in black:
                    #print('      adjacent', test_pos)
                    count += 1
            #print('    count', count)
            if (ew, ns) in black:
                if count in (1, 2):
                    #print('    black->black')
                    new_black[pos] = 1
                else:
                    #print('    black->white')
                    pass
            else:
                if count == 2:
                    #print('    white->black')
                    new_black[pos] = 1
                else:
                    #print('    white->white')
                    pass
    black = new_black
    print(f'  day {day} black tiles now {len(black)}')
