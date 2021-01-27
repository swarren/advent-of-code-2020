#!/usr/bin/env python3

with open("../input/day18.txt") as f:
    data = f.read().splitlines()

def parse_expr(s, i, indent):
    print(indent + f'parse_expr("{s}", "{s[i:]}", {i})')
    result, i = parse_term(s, i, indent + '  ')
    while True:
        if i >= len(s) or s[i] != '*':
            break
        # consume s[i] == '*'
        i += 1
        subresult, i = parse_term(s, i, indent + '  ')
        result *= subresult
    print(indent + f'-> {result} {i}, "{s[i:]}"')
    return result, i

def parse_term(s, i, indent):
    print(indent + f'parse_term("{s}", "{s[i:]}", {i})')
    result, i = parse_val(s, i, indent + '  ')
    while True:
        if i >= len(s) or s[i] != '+':
            break
        # consume s[i] == '+'
        i += 1
        subresult, i = parse_val(s, i, indent + '  ')
        result += subresult
    print(indent + f'-> {result} {i}, "{s[i:]}"')
    return result, i

def parse_val(s, i, indent):
    print(indent + f'parse_val("{s}", "{s[i:]}", {i})')
    if s[i] == '(':
        # consume s[i] == '('
        i += 1
        result, i = parse_expr(s, i, indent + '  ')
        # consume s[i] == ')'
        i += 1
    else:
        result = int(s[i])
        # consume s[i] == a digit
        i += 1
    print(indent + f'-> {result} {i}, "{s[i:]}"')
    return result, i

def calc(s):
    return parse_expr(s, 0, '')[0]

#print(calc('1+(2*3)+(4*(5+6))'))
#print(calc('2*3+(4*5)'))
#print(calc('5+(8*3+9+3*4*3)'))
#print(calc('5*9*(7*3*3+9*3+(8+6*4))'))
#print(calc('((2+4*9)*(6+9*8+6)+6)+2+4*2'))

result = 0
for expr in data:
    result += calc(expr.replace(' ', ''))
print(result)
