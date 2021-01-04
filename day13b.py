#!/usr/bin/env python3

with open("day13.txt") as f:
    data = f.read()
lines = data.splitlines()
time = int(lines[0])
busses = lines[1].split(',')

busses = list(map(lambda x: x != 'x' and int(x) or None, busses))

"""
#b0_mult = 0
b0_mult = (100000000000000 // busses[0]) - 1
while True:
    b0_mult += 1
    b0_time = busses[0] * b0_mult
    match = True
    for i, bus in enumerate(busses[1:]):
        if bus is None:
            continue
        bn_time = b0_time + i + 1
        if bn_time % bus:
            match = False
            break
    if match:
        break
print(b0_mult * busses[0])
"""

# "surely the actual earliest timestamp will be larger than 100000000000000"
# approx hack to know where to start:
n = 100000000000000 // (937 * 569)
# time with above n: 13s w/pypy3, time with n==1: 16s w/pypyp3
while True:
    n += 1
    # two biggest bus numbers: 569@13, 937@44, 44-13==31
    # Wolfram Alpha:
    # 569a+31=937b over integers -> a = 937 n + 23 and b = 569 n + 14 and n element Z
    b0_time = (937 * (569 * n + 14)) - 44
    #print(f'b0_time {b0_time}')
    match = True
    for i, bus in enumerate(busses):
        if bus is None:
            continue
        bn_time = b0_time + i
        #print(f'  [{i}] {bn_time} {bus}')
        if bn_time % bus:
            #print(f'    no match %={bn_time % bus}')
            match = False
            break
        #print(f'    match!')
    if match:
        break
print(b0_time)
