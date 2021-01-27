#!/usr/bin/env python3

plen = 25
with open("../input/day9.txt") as f:
    data = f.read()
data = list(map(int, data.splitlines()))

def check(val, preamble):
    return True in [(x != y) and (x + y) == val for x in preamble for y in preamble]

for i in range(plen, len(data)):
    if not check(data[i], data[i - plen:i]):
        print(data[i])
        break
