#!/usr/bin/env python3

import data9 as data

def check(val, preamble):
    return True in [(x != y) and (x + y) == val for x in preamble for y in preamble]

for i in range(data.plen, len(data.data)):
    if not check(data.data[i], data.data[i - data.plen:i]):
        found_i = i
        found_val = data.data[i]
        print(found_i, found_val)
        break

candidates = data.data[:found_i]
for c_s in range(len(candidates)):
    for c_e in range(c_s, len(candidates)):
        l = candidates[c_s:c_e]
        if sum(l) == found_val:
            print(repr(l), min(l) + max(l))
            break
