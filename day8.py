#!/usr/bin/env python3

import data8 as data
import re

instructions = []
for entry in data.data.splitlines():
    #print(entry)
    op, val = entry.strip().split()
    val = int(val)
    instructions.append((op, val))

def op_acc(val):
    global acc
    global pc
    acc += val
    pc += 1

def op_jmp(val):
    global pc
    pc += val

def op_nop(val):
    global pc
    pc += 1

opfuncs = {
    "acc": op_acc,
    "jmp": op_jmp,
    "nop": op_nop,
}

seen = [False] * len(instructions)
acc = 0
pc = 0
while True:
    if seen[pc]:
        break
    seen[pc] = True
    op, val = instructions[pc]
    opfuncs[op](val)
print(acc)
