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

rules[8] = [[42], [42, 8]]
rules[11] = [[42, 31], [42, 11, 31]]

def match_rule_list(s, i, rule_list, indent):
    #print(indent + f'match_rule_list(s={s}, i={i}, rule_list={rule_list}, sleft={s[i:]})')
    if not rule_list:
        yield i
        return
    for si in match_rule(s, i, rule_list[0], indent):
        yield from match_rule_list(s, si, rule_list[1:], indent)

def match_rule(s, i, ruleno, indent):
    #print(indent + f'match_rule(s={s}, i={i}, ruleno={ruleno}, sleft={s[i:]})')
    for rule_list in rules[ruleno]:
        #print(indent + f'  rule_list={rule_list}')
        if isinstance(rule_list[0], str):
            if i < len(s) and s[i] == rule_list[0]:
                yield i + 1
            return
        yield from match_rule_list(s, i, rule_list, indent + '  ')

def is_match(s):
    return len(s) in match_rule(s, 0, 0, '')

count = 0
for s in messages:
    m = is_match(s)
    #print(s, m)
    if m:
        count += 1
print(count)
