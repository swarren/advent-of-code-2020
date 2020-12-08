#!/usr/bin/env python3

import data8 as data
import re

instructions = []
for entry in data.data.splitlines():
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

ss = [False] * len(instructions)
seens = []
for i in range(len(programs)):
    seens.append(ss[:])
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
        if seens[program][pc]:
            #print('    Seen')
            continue
        seens[program][pc] = True
        opfuncs[op](val)
        pcs[program] = pc
        accs[program] = acc
        #print('    ->', pc, acc)
        if pc == len(instructions):
            print('RESULT', acc)
            break
    if pc == len(instructions):
        break
