#!/usr/bin/env python3

with open("day6.txt") as f:
    data = f.read()

count = 0
people_count = 0
answers = [0] * 26

def finish_group():
    global count, people_count, answers
    person_count = len([pc for pc in answers if pc == people_count])
    count += person_count
    people_count = 0
    answers = [0] * 26

for entry in data.splitlines():
    entry = entry.strip()
    if not entry:
        finish_group()
        continue
    people_count += 1
    for i in range(26):
        c = chr(ord('a') + i)
        if c in entry:
            answers[i] += 1
finish_group()
print(count)

