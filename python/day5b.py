#!/usr/bin/env python3

with open("../input/day5.txt") as f:
    data = f.read()

def partition(ones, s):
    val = 0
    for c in s:
        val <<= 1
        if c in ones:
            val |= 1
    return val

hsn = 0
found = {}
for entry in data.splitlines():
    sn = partition("BR", entry)
    found[sn] = True
    hsn = max(hsn, sn)
found_full = False
for i in range(hsn):
    if i in found:
        found_full = True
    elif found_full:
        print(i)
        break
