#!/usr/bin/env python3

import data16 as data

ok_tickets = []
for ticket in data.nearby_tickets:
    ticket_match = True
    for val in ticket:
        val_match = False
        for name, minmaxes in data.fields:
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
for name, minmaxes in data.fields:
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
for i, (name, minmaxes) in enumerate(data.fields):
    if not name.startswith('departure'):
        continue
    result *= data.my_ticket[field_to_position[i]]
print(result)
