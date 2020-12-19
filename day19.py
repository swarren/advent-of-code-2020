#!/usr/bin/env python3

import data19 as data

def match(s, i, ruleno, indent):
    #print(indent + f'match(s={s}, i={i}, ruleno={ruleno}, sleft={s[i:]})')
    for option in data.rules[ruleno]:
        optionmatch = True
        optioni = i
        for nest in option:
            if isinstance(nest, str):
                if s[optioni] != nest:
                    optionmatch = False
                    break
                optioni += 1
            else:
                sm, optioni = match(s, optioni, nest, indent + '  ')
                if not sm:
                    optionmatch = False
                    break
        if optionmatch:
            #print(indent + f'-> Matches, iret={optioni}')
            return True, optioni
    #print(indent + '-> No')
    return False, i

def is_match(s):
    m, l = match(s, 0, 0, '')
    return m and l == len(s)

count = 0
for s in data.strings:
    m = is_match(s)
    print(s, m)
    if m:
        count += 1
print(count)
