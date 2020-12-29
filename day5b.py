#!/usr/bin/env python3

with open("day5.txt") as f:
    data = f.read()

def partition(zc, sz, s):
    val = 0
    for c in s:
        if c != zc:
            val += sz // 2
        sz = sz // 2
    return val

hsn = 0
found = {}
for entry in data.splitlines():
    fb = entry[:7]
    fbn = partition('F', 128, fb)
    lr = entry[7:]
    lrn = partition('L', 8, lr)
    sn = (fbn * 8) + lrn
    found[sn] = True
    hsn = max(hsn, sn)
found_full = False
for i in range(hsn):
    if i in found:
        found_full = True
    elif found_full:
        print(i)
        break
