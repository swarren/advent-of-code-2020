#!/usr/bin/env python3

# Mine
cups = list('364297581')
# Example
#cups = list('389125467')

cups = list(map(int, cups))

cup_count = 1_000_000
cups.extend(range(len(cups) + 1, cup_count + 1))

min_cup = 1
max_cup = cup_count

curr_cup_index = 0
for move in range(1, 10_000_001):
    curr_cup = cups[curr_cup_index]
    pick_after = min(3, len(cups) - curr_cup_index - 1)
    pick_start = 3 - pick_after
    pick_up = cups[curr_cup_index + 1:curr_cup_index + pick_after + 1] + cups[:pick_start]
    cups[curr_cup_index + 1:curr_cup_index + pick_after + 1] = []
    cups[:pick_start] = []
    find_cup = curr_cup

    destination_index = None
    while destination_index is None:
        find_cup -= 1
        if find_cup < min_cup:
            find_cup = max_cup
        try:
            destination_index = cups.index(find_cup)
            break
        except:
            pass
    destination = cups[destination_index]
    cups[destination_index + 1:destination_index + 1] = pick_up

    curr_cup_index = cups.index(curr_cup)
    curr_cup_index = (curr_cup_index + 1) % len(cups)

index_1 = cups.index(1)
cup_after_1 = cups[(index_1 + 1) % len(cups)]
cup_after_2 = cups[(index_1 + 2) % len(cups)]
print(cup_after_1, cup_after_2, cup_after_1 * cup_after_2)
