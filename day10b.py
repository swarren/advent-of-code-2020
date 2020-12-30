#!/usr/bin/env python3

with open("day10.txt") as f:
    data = f.read()
data = list(map(int, data.splitlines()))

l = sorted(data)
l[0:0] = [0]
l.append(l[-1] + 3)
diffs = [l[i] - l[i - 1] for i in range(1, len(l))]
#print(repr(diffs))

seqs = []
ld = None
cnt = 0
for d in diffs:
    if d == ld:
        cnt += 1
    else:
        if ld == 1:
            seqs.append(cnt)
        ld = d
        cnt = 1
#print(repr(seqs))

def collapses(length, indent=''):
    #print(indent + f"collapses({length})")
    if length < 2:
        ret = 1
    else:
        ret = 1
        #print(indent + "  2s:")
        for i in range(length - 1):
            ret += 1 * collapses(length - i - 2, indent + '    ')
        #print(indent + "  3s:")
        for i in range(length - 2):
            ret += 1 * collapses(length - i - 3, indent + '    ')
    #print(indent + f"ret({ret})")
    return ret

combinations = 1
for s in seqs:
    combinations *= collapses(s)
print(combinations)
