#!/usr/bin/env python3

import data7 as data
import re

rules = {}
for entry in data.data.splitlines():
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
    rules[outer] = inners

def nest(color, indent):
    inners = rules[color]
    #print(indent, color, 'rules', inners)
    c = 1
    for count, inner in inners:
        #print(indent, '   ', count, inner)
        inner_nest = nest(inner, indent + '    ')
        c += count * inner_nest
        #print(indent, count, inner_nest, count * inner_nest, c)
    #print(indent, c)
    return c
print(nest('shiny gold', '') - 1)
