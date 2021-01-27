#!/usr/bin/env python3

# Concept stolen from:
# https://www.reddit.com/r/adventofcode/comments/kimluc/2020_day_23_solutions/ggrxrh6/?utm_source=share&utm_medium=web2x&context=3
# ... but re-implemented from memory after reading that.

# Mine
cups = list('364297581')
# Example
#cups = list('389125467')

cups = list(map(int, cups))

cup_count = 1_000_000
cup_next = [0] * (cup_count + 1)
for i, cup in enumerate(cups[:-1]):
    cup_next[cup] = cups[i + 1]
cup_next[cups[-1]] = len(cups) + 1
for i in range(len(cups) + 1, cup_count):
    cup_next[i] = i + 1
cup_next[cup_count] = cups[0]

cup = cups[0]
for move in range(1, 10_000_001):
    pick_up = []
    pick_up.append(cup_next[cup])
    pick_up.append(cup_next[pick_up[0]])
    pick_up.append(cup_next[pick_up[1]])
    cup_next[cup] = cup_next[pick_up[2]]

    destination_cup = cup
    while True:
        destination_cup -= 1
        if destination_cup < 1:
            destination_cup = cup_count
        if destination_cup not in pick_up:
            break
    cup_next[pick_up[2]] = cup_next[destination_cup]
    cup_next[destination_cup] = pick_up[0]

    cup = cup_next[cup]

cup_after_1 = cup_next[1]
cup_after_2 = cup_next[cup_after_1]
print(cup_after_1, cup_after_2, cup_after_1 * cup_after_2)
