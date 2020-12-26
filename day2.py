#!/usr/bin/env python3

with open("day2.txt") as f:
    data = f.read()

count = 0
for entry in data.splitlines():
    policy, password = entry.split(':')
    password = password.strip()
    minmax, character = policy.split(' ')
    nmin, nmax = map(int, minmax.split('-'))
    without = password.replace(character, '')
    delta = len(password) - len(without)
    if nmin <= delta <= nmax:
        count += 1
print(count)
