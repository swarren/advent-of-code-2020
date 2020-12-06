#!/usr/bin/env python3

import data5 as data

def partition(zc, sz, s):
    val = 0
    for c in s:
        if c != zc:
            val += sz // 2
        sz = sz // 2
    return val

hsn = -1
for entry in data.data.splitlines():
    fb = entry[:7]
    fbn = partition('F', 128, fb)
    lr = entry[7:]
    lrn = partition('L', 8, lr)
    sn = (fbn * 8) + lrn
    hsn = max(hsn, sn)
print(hsn)
