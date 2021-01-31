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

hsn = -1
for entry in data.splitlines():
    sn = partition("BR", entry)
    hsn = max(hsn, sn)
print(hsn)
