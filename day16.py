#!/usr/bin/env python3

import data16 as data

error_rate = 0
for ticket in data.nearby_tickets:
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
            error_rate += val
print(error_rate)
