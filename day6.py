#!/usr/bin/env python3

with open("day6.txt") as f:
    data = f.read()

count = 0
answers = [False] * 26

def finish_group():
    global count, answers
    person_count = len([a for a in answers if a])
    count += person_count
    answers = [False] * 26

for entry in data.splitlines():
    entry = entry.strip()
    if not entry:
        finish_group()
        continue
    for i in range(26):
        c = chr(ord('a') + i)
        if c in entry:
            answers[i] = True
finish_group()
print(count)
