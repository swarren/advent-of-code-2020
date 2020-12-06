#!/usr/bin/env python3

import data1 as data

ds = sorted(data.data)
for i, x in enumerate(ds):
    for j, y in enumerate(ds):
        if j >= i:
            continue
        for k, z in enumerate(ds):
            if k >= j:
                continue
            if x + y + z == 2020:
                print(x, y, z, x * y * z)
                break
