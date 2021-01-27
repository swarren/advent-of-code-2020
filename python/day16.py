#!/usr/bin/env python3

fields = []
my_ticket = None
nearby_tickets = []

with open("../input/day16.txt") as f:
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

error_rate = 0
for ticket in nearby_tickets:
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
            error_rate += val
print(error_rate)
