#!/usr/bin/env python3

with open("day3.txt") as f:
    data = f.read()

dx = 3

count = 0
xp = 0
for entry in data.splitlines():
    if entry[xp % len(entry)] == '#':
        count += 1
    xp += dx
print(count)
