#!/usr/bin/env python3

rules = {}
messages = []
def parse_item(s):
    if s[0] == '"':
        return s[1]
    return int(s)
with open("../input/day19.txt") as f:
    # 117: "b"
    # 68: 33 99 | 117 35
    while True:
        l = f.readline().strip()
        if not l:
            break
        (s_rnum, s_options) = l.split(': ')
        ls_options = s_options.split(' | ')
        options = []
        for s_option in ls_options:
            options.append(list(map(parse_item, s_option.split())))
        rules[int(s_rnum)] = options
    # abbbaaaaaabbbaabababaaab
    while True:
        l = f.readline().strip()
        if not l:
            break
        messages.append(l)

def match(s, i, ruleno, indent):
    #print(indent + f'match(s={s}, i={i}, ruleno={ruleno}, sleft={s[i:]})')
    for option in rules[ruleno]:
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
for s in messages:
    m = is_match(s)
    #print(s, m)
    if m:
        count += 1
print(count)
