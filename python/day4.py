#!/usr/bin/env python3

with open("../input/day4.txt") as f:
    data = f.read()

required_fields = sorted((
    'byr',
    'iyr',
    'eyr',
    'hgt',
    'hcl',
    'ecl',
    'pid',
))

count = 0
found_fields = {}

def finish_fields():
    global count
    global found_fields

    if 'cid' in found_fields:
        del found_fields['cid']
    if sorted(found_fields.keys()) == required_fields:
        count += 1
    found_fields = {}

for entry in data.splitlines():
    entry = entry.strip()
    if not entry:
        finish_fields()
        continue
    found_fields.update({x.split(':')[0]: True for x in entry.split(' ')})
finish_fields()
print(count)
