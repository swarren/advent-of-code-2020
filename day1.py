#!/usr/bin/env python3

import data1 as data

ds = sorted(data.data)
for i, x in enumerate(ds):
    for j, y in enumerate(ds):
        if j >= i:
            continue
        if x + y == 2020:
            print(x, y, x * y)
            break
