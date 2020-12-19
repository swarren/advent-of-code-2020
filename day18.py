#!/usr/bin/env python3

import data18 as data

def calc(s):
    i = 0

    if s[0] == '(':
        result, di = calc(s[1:])
        i += 1 + di
    else:
        result = int(s[0])
        i += 1

    op = None
    while i < len(s):
        c = s[i]
        if c == ')':
            return result, i + 1
        if c == '+':
            op = lambda x, y: x + y
            i += 1
            continue
        elif c == '*':
            op = lambda x, y: x * y
            i += 1
            continue
        if c == '(':
            n, di = calc(s[i+1:])
            i += 1 + di
        else:
            n = int(c)
            i += 1
        result = op(result, n)
    return result, i

result = 0
for expr in data.data.splitlines():
    val, _ = calc(expr.replace(' ', ''))
    result += val
print(result)
