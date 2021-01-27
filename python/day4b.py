#!/usr/bin/env python3

import re

with open("../input/day4.txt") as f:
    data = f.read()

def valid_byr(s):
    try:
        if len(s) != 4:
            return
        i = int(s)
        if i < 1920 or i > 2002:
            return False
        return True
    except:
        return False

def valid_iyr(s):
    try:
        if len(s) != 4:
            return
        i = int(s)
        if i < 2010 or i > 2020:
            return False
        return True
    except:
        return False

def valid_eyr(s):
    try:
        if len(s) != 4:
            return
        i = int(s)
        if i < 2020 or i > 2030:
            return False
        return True
    except:
        return False

def valid_hgt(s):
    try:
        if s.endswith('cm'):
            i = int(s[:-2])
            if i < 150 or i > 193:
                return False
            return True
        if s.endswith('in'):
            i = int(s[:-2])
            if i < 59 or i > 76:
                return False
            return True
        return False
    except:
        return False

re_hcl = re.compile('^#[0-9a-f]{6}$')
def valid_hcl(s):
    return bool(re_hcl.match(s))

def valid_ecl(s):
    return s in ['amb', 'blu', 'brn', 'gry', 'grn', 'hzl', 'oth']

re_pid = re.compile('^[0-9]{9}$')
def valid_pid(s):
    return bool(re_pid.match(s))

required_fields = {
    'byr': valid_byr,
    'iyr': valid_iyr,
    'eyr': valid_eyr,
    'hgt': valid_hgt,
    'hcl': valid_hcl,
    'ecl': valid_ecl,
    'pid': valid_pid,
}

count = 0
found_fields = {}

def finish_fields():
    global count
    global found_fields

    if 'cid' in found_fields:
        del found_fields['cid']
    if len(found_fields) == len(required_fields):
        valid = True
        for field, is_valid in required_fields.items():
            if not is_valid(found_fields[field]):
                valid = False
                break
        if valid:
            count += 1
    found_fields = {}

for entry in data.splitlines():
    entry = entry.strip()
    if not entry:
        finish_fields()
        continue
    found_fields.update({x.split(':')[0]: x.split(':')[1] for x in entry.split(' ')})
finish_fields()
print(count)
