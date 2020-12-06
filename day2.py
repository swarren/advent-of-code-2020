#!/usr/bin/env python3

import data2 as data

count = 0
for entry in data.data.splitlines():
    policy, password = entry.split(':')
    password = password.strip()
    minmax, character = policy.split(' ')
    nmin, nmax = map(int, minmax.split('-'))
    without = password.replace(character, '')
    delta = len(password) - len(without)
    if nmin <= delta <= nmax:
        count += 1
print(count)
