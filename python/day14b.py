#!/usr/bin/env python3

import re

with open("../input/day14.txt") as f:
    data = f.read()

mem = {}
def write(addr, mask, value, addri=0):
    if len(addr) == 0:
        mem[addri] = value
        return

    if mask[0] == '0':
        if addr[0] == '0':
            addr_bits = [0]
        else:
            addr_bits = [1]
    elif mask[0] == '1':
        addr_bits = [1]
    else:
        addr_bits = [0, 1]
    for addr_bit in addr_bits:
        write(addr[1:], mask[1:], value, (addri << 1) | addr_bit)

mask = None
for l in data.splitlines():
    if l.startswith('mask'):
        mask = l.split()[2]
    else:
        addrspec, equals, value = l.split()
        addrd = addrspec.split('[')[1].split(']')[0]
        value = int(value)
        addri = int(addrd)
        addr = f'{addri:036b}'
        write(addr, mask, value)

print(sum(mem.values()))
