#!/usr/bin/env python3

from itertools import chain

class Node:
    def __init__(self, label, next):
        self.label = label
        self.next = next

# Mine
cups = list('364297581')
# Example
#cups = list('389125467')

cups = list(map(int, cups))
cup_count = 1_000_000
min_cup = 1
max_cup = cup_count

cups.extend(range(len(cups) + 1, cup_count + 1))

last_cup = None
prev_cup = None
for cup_label in chain(range(cup_count + 1, len(cups) -1), cups[::-1]):
    cup_node = Node(cup_label, prev_cup)
    if last_cup is None:
        last_cup = cup_node
    prev_cup = cup_node
last_cup.next = cup_node
curr_cup = cup_node

for move in range(1, 10_000_001):
    #print(f'-- move {move} --')
    #print('cups: ', end='')
    #cup = curr_cup
    #while True:
    #    if cup == curr_cup:
    #        print('(', end='')
    #    print(cup.label, end='')
    #    if cup == curr_cup:
    #        print(')', end='')
    #    print(' ', end='')
    #    cup = cup.next
    #    if cup == curr_cup:
    #        break
    #print()

    pick_up_cups = [
        curr_cup.next,
        curr_cup.next.next,
        curr_cup.next.next.next,
    ]
    curr_cup.next = pick_up_cups[-1].next

    pick_up_labels = list(map(lambda cup: cup.label, pick_up_cups))
    #print('pick up:', ', '.join(map(str, pick_up_labels)))
    destination_label = curr_cup.label
    while True:
        destination_label -= 1
        if destination_label < min_cup:
            destination_label = max_cup
        if destination_label not in pick_up_labels:
            break
    #print('destination_label:', destination_label)
    #print()

    destination_cup = curr_cup
    while destination_cup.label != destination_label:
        destination_cup = destination_cup.next
    pick_up_cups[-1].next = destination_cup.next
    destination_cup.next = pick_up_cups[0]

    curr_cup = curr_cup.next

#print('-- final --')
#print('cups: ', end='')
#cup = curr_cup
#while True:
#    print(cup.label, end=' ')
#    cup = cup.next
#    if cup == curr_cup:
#        break
#print()

while curr_cup.label != 1:
    curr_cup = curr_cup.next
cup_after_1 = curr_cup.next.label
cup_after_2 = curr_cup.next.next.label
print(cup_after_1, cup_after_2, cup_after_1 * cup_after_2)
