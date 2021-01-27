#!/usr/bin/env python3

with open("../input/day1.txt") as f:
    data = map(int, f.read().splitlines())

ds = sorted(data)
for i, x in enumerate(ds):
    for j, y in enumerate(ds):
        if j >= i:
            continue
        for k, z in enumerate(ds):
            if k >= j:
                continue
            if x + y + z == 2020:
                print(x * y * z)
                break
