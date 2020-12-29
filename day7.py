#!/usr/bin/env python3

import re

with open("day7.txt") as f:
    data = f.read()

rules = []
for entry in data.splitlines():
    #print(entry)
    entry = entry.strip()
    outer, inner_text = entry[:-1].split('contain')
    outer = outer.strip()[:-5]
    #print(' ', outer)
    inners = []
    if 'no other bags' not in inner_text:
        for x in inner_text.split(','):
            #print(' ', x)
            fields = x.strip().split(' ')
            count = int(fields[0])
            inner = ' '.join(fields[1:-1])
            inners.append((count, inner))
    #print(' ', repr(inners))
    rules.append((outer, inners))

valid_outers = {}
to_check = ['shiny gold']
while len(to_check):
    #print('check', to_check[0])
    for outer, inners in rules:
        #print('  rule', outer, inners)
        if outer in valid_outers:
            #print('    in valid_outers')
            continue
        inner_colors = [color for count, color in inners]
        #print('   ', inner_colors)
        if to_check[0] in inner_colors:
            #print('    found')
            to_check.append(outer)
            valid_outers[outer] = True
        #else:
        #    print('   ', to_check[0], ' not in that ^^')
    to_check = to_check[1:]
print(len(valid_outers))
