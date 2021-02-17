#!/usr/bin/env python3

# Mine
cups = list('364297581')
# Example
#cups = list('389125467')

cups = list(map(int, cups))
min_cup = min(cups)
max_cup = max(cups)

curr_cup_index = 0
for move in range(1, 101):
    #print(f'-- move {move} --')
    #print('cups: ', end='')
    #for i, cup in enumerate(cups):
    #    if i == curr_cup_index:
    #        print('(', end='')
    #    print(cup, end='')
    #    if i == curr_cup_index:
    #        print(')', end='')
    #    print(' ', end='')
    #print()

    curr_cup = cups[curr_cup_index]
    pick_after = min(3, len(cups) - curr_cup_index - 1)
    pick_start = 3 - pick_after
    pick_up = cups[curr_cup_index + 1:curr_cup_index + pick_after + 1] + cups[:pick_start]
    cups[curr_cup_index + 1:curr_cup_index + pick_after + 1] = []
    cups[:pick_start] = []

    #print('pick up:', ', '.join(map(str, pick_up)))
    #print('left:', ', '.join(map(str, cups)))

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
    #print('destination:', destination, 'index', destination_index)
    #print()

    cups[destination_index + 1:destination_index + 1] = pick_up

    curr_cup_index = cups.index(curr_cup)
    curr_cup_index = (curr_cup_index + 1) % len(cups)

#print('-- final -- ')
#print(cups)
index_1 = cups.index(1)
print(''.join(map(str, cups[index_1 + 1:] + cups[:index_1])))
