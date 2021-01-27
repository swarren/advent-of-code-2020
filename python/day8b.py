#!/usr/bin/env python3

import re

with open("../input/day8.txt") as f:
    data = f.read()

instructions = []
for entry in data.splitlines():
    #print(entry)
    op, val = entry.strip().split()
    val = int(val)
    instructions.append([op, val])

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

programs = []
for corrupt in range(len(instructions)):
    if instructions[corrupt][0] == 'jmp':
        mod_instructions = instructions[:]
        mod_instructions[corrupt] = ['nop', mod_instructions[corrupt][1]]
        programs.append(mod_instructions)
    elif instructions[corrupt][0] == 'nop':
        mod_instructions = instructions[:]
        mod_instructions[corrupt] = ['jmp', mod_instructions[corrupt][1]]
        programs.append(mod_instructions)
#print(repr(programs))

accs = [0] * len (programs)
pcs = [0] * len(programs)
ctr = 0
while True:
    #print('Instruction #', ctr)
    ctr += 1
    for program in range(len(programs)):
        #print('  Program #', program)
        pc = pcs[program]
        acc = accs[program]
        op, val = programs[program][pc]
        #print('   ', op, val, pc, acc)
        opfuncs[op](val)
        pcs[program] = pc
        accs[program] = acc
        #print('    ->', pc, acc)
        if pc == len(instructions):
            print(acc)
            break
    if pc == len(instructions):
        break
