#!/usr/bin/env python3

plen = 25
with open("day9.txt") as f:
    data = f.read()
data = list(map(int, data.splitlines()))

def check(val, preamble):
    return True in [(x != y) and (x + y) == val for x in preamble for y in preamble]

for i in range(plen, len(data)):
    if not check(data[i], data[i - plen:i]):
        found_i = i
        found_val = data[i]
        #print(found_i, found_val)
        break

candidates = data[:found_i]
for c_s in range(len(candidates)):
    for c_e in range(c_s, len(candidates)):
        l = candidates[c_s:c_e]
        if sum(l) == found_val:
            print(min(l) + max(l))
            break
