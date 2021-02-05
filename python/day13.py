#!/usr/bin/env python3

with open("../input/day13.txt") as f:
    data = f.read()
lines = data.splitlines()
time = int(lines[0])
busses = lines[1].split(',')

busses = map(int, filter(lambda x: x != 'x', busses))

earliest = 0
earliest_bus = 0
for bus in busses:
    d = time // bus
    r = time % bus
    if r:
        bus_time = bus * (d + 1)
    else:
        bus_time = time
    #print(bus, bus_time)
    if (not earliest) or (bus_time < earliest):
        earliest = bus_time
        earliest_bus = bus
        #print('earliest', earliest)
print((earliest - time) * earliest_bus)
