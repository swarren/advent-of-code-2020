#!/usr/bin/env python3

import data3 as data

slopes = (
  (1, 1),
  (3, 1),
  (5, 1),
  (7, 1),
  (1, 2),
)
dx = 3

answer = 1
for dx, dy in slopes:
    count = 0
    xp = 0
    for yp, entry in enumerate(data.data.splitlines()):
        if yp % dy:
            continue
        if entry[xp % len(entry)] == '#':
            count += 1
        xp += dx
    answer *= count
print(answer)