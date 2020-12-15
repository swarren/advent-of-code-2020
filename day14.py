#!/usr/bin/env python3

import data14 as data
import re

mem = {}
mask_or = None
mask_and = None
for l in data.data.splitlines():
    if l.startswith('mask'):
        mask = l.split()[2]
        mask_or = 0
        mask_and = 0
        for c in mask:
            mask_or <<= 1
            mask_and <<= 1
            if c == '1':
                mask_or |= 1
            elif c == '0':
                pass
            else:
                mask_and |= 1
    else:
        addrspec, equals, value = l.split()
        addr = addrspec.split('[')[1].split(']')[0]
        value = int(value)
        addr = int(addr)
        mem[addr] = (value & mask_and) | mask_or        
print(sum(mem.values()))
