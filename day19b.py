#!/usr/bin/env python3

import data19 as data

data.rules[8] = [[42], [42, 8]]
data.rules[11] = [[42, 31], [42, 11, 31]]

def match_rule_list(s, i, rule_list, indent):
    #print(indent + f'match_rule_list(s={s}, i={i}, rule_list={rule_list}, sleft={s[i:]})')
    if not rule_list:
        yield i
        return
    for si in match_rule(s, i, rule_list[0], indent):
        yield from match_rule_list(s, si, rule_list[1:], indent)

def match_rule(s, i, ruleno, indent):
    #print(indent + f'match_rule(s={s}, i={i}, ruleno={ruleno}, sleft={s[i:]})')
    for rule_list in data.rules[ruleno]:
        #print(indent + f'  rule_list={rule_list}')
        if isinstance(rule_list, str):
            if i < len(s) and s[i] == rule_list:
                yield i + 1
            return
        yield from match_rule_list(s, i, rule_list, indent + '  ')

def is_match(s):
    return len(s) in match_rule(s, 0, 0, '')

count = 0
for s in data.strings:
    m = is_match(s)
    print(s, m)
    if m:
        count += 1
print(count)
