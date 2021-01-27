#!/usr/bin/env python3

with open("../input/day2.txt") as f:
    data = f.read()

count = 0
for entry in data.splitlines():
    policy, password = entry.split(':')
    password = password.strip()
    minmax, character = policy.split(' ')
    pos1, pos2 = map(int, minmax.split('-'))
    pos1_match = (pos1 <= len(password)) and (password[pos1 - 1] == character)
    pos2_match = (pos2 <= len(password)) and (password[pos2 - 1] == character)
    if pos1_match ^ pos2_match:
        count += 1
print(count)
