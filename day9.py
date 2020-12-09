#!/usr/bin/env python3

import data9 as data

def check(val, preamble):
    return True in [(x != y) and (x + y) == val for x in preamble for y in preamble]

for i in range(data.plen, len(data.data)):
    if not check(data.data[i], data.data[i - data.plen:i]):
        print(i, data.data[i])
        break
