#!/usr/bin/env python3

import data6 as data

count = 0
answers = [False] * 26
for entry in data.data.splitlines():
    entry = entry.strip()
    if not entry:
        person_count = len([a for a in answers if a])
        count += person_count
        answers = [False] * 26
        continue
    for i in range(26):
        c = chr(ord('a') + i)
        if c in entry:
            answers[i] = True
print(count)
