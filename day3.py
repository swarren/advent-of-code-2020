#!/usr/bin/env python3

import data3 as data

dx = 3

count = 0
xp = 0
for entry in data.data.splitlines():
    if entry[xp % len(entry)] == '#':
        count += 1
    xp += dx
print(count)
