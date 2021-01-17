#!/usr/bin/env python3

fields = []
my_ticket = None
nearby_tickets = []

with open("day16.txt") as f:
    # departure location: 43-237 or 251-961
    while True:
        l = f.readline().strip()
        if not l:
            break
        field, rstring = l.split(': ')
        fields.append([field, []])
        rstrings = rstring.split(' or ')
        for rstring in rstrings:
            min_max = list(map(int, rstring.split('-')))
            fields[-1][1].append(min_max)
    # your ticket:
    f.readline()
    # 151,71,67,113,127,163,131,59,137,103,73,139,107,101,97,149,157,53,109,61
    while True:
        l = f.readline().strip()
        if not l:
            break
        my_ticket = list(map(int, l.split(',')))
    # nearby tickets:
    f.readline()
    # 680,418,202,55,792,800,896,801,312,252,721,702,24,112,608,837,98,222,797,364
    while True:
        l = f.readline().strip()
        if not l:
            break
        nearby_tickets.append(list(map(int, l.split(','))))


ok_tickets = []
for ticket in nearby_tickets:
    ticket_match = True
    for val in ticket:
        val_match = False
        for name, minmaxes in fields:
            for minmax in minmaxes:
                if minmax[0] <= val <= minmax[1]:
                    val_match = True
                    break
            if val_match:
                break
        if not val_match:
            ticket_match = False
            break
    if ticket_match:
        ok_tickets.append(ticket)

field_positions = []
for name, minmaxes in fields:
    legal_positions = []
    for position in range(len(ok_tickets[0])):
        pos_match = True
        for ticket in ok_tickets:
            val_match = False
            val = ticket[position]
            for minmax in minmaxes:
                if minmax[0] <= val <= minmax[1]:
                    val_match = True
                    break
            if not val_match:
                pos_match = False
                break
        if pos_match:
            legal_positions.append(position)
    field_positions.append(legal_positions)

field_to_position = [None] * len(ok_tickets[0])
while None in field_to_position:
    for i, legal_positions in enumerate(field_positions):
        if len(legal_positions) == 1:
            assigned_pos = legal_positions[0]
            field_to_position[i] = assigned_pos
            for i, legal_positions in enumerate(field_positions):
                if assigned_pos in legal_positions:
                    legal_positions.remove(assigned_pos)
            break

result = 1
for i, (name, minmaxes) in enumerate(fields):
    if not name.startswith('departure'):
        continue
    result *= my_ticket[field_to_position[i]]
print(result)
