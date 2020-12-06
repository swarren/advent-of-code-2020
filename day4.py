#!/usr/bin/env python3

import data4 as data

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
for entry in data.data.splitlines():
    entry = entry.strip()
    if not entry:
        if 'cid' in found_fields:
            del found_fields['cid']
        if sorted(found_fields.keys()) == required_fields:
            count += 1
        found_fields = {}
        continue
    found_fields.update({x.split(':')[0]: True for x in entry.split(' ')})
print(count)
