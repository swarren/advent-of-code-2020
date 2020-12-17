#!/usr/bin/env python3

import data15 as data

hist = {}

for pos, val in enumerate(data.data):
    #print(pos, val)
    hist[val] = [pos]
    prev_val = val

for i in range(len(data.data), 2020):
    last_mentions = hist[prev_val]
    #print('    prev_val', prev_val, 'last_mentions', last_mentions)
    if len(last_mentions) < 2:
        to_insert = 0
    else:
        to_insert = last_mentions[1] - last_mentions[0]
    last_mentions = hist.get(to_insert, [])[-1:] + [i]
    #print('      to_insert', to_insert, 'last_mentions', last_mentions)
    #print(i, to_insert)
    hist[to_insert] = last_mentions
    prev_val = to_insert
print(prev_val)
