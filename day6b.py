#!/usr/bin/env python3

import data6 as data

count = 0
people_count = 0
answers = [0] * 26
for entry in data.data.splitlines():
    entry = entry.strip()
    if not entry:
        person_count = len([pc for pc in answers if pc == people_count])
        count += person_count
        people_count = 0
        answers = [0] * 26
        continue
    people_count += 1
    for i in range(26):
        c = chr(ord('a') + i)
        if c in entry:
            answers[i] += 1
print(count)

