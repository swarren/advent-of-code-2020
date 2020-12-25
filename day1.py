#!/usr/bin/env python3

with open("day1.txt") as f:
    data = map(int, f.read().splitlines())

ds = sorted(data)
for i, x in enumerate(ds):
    for j, y in enumerate(ds):
        if j >= i:
            continue
        if x + y == 2020:
            print(x * y)
            break
