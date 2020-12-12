#!/usr/bin/env python3

import data10 as data

l = sorted(data.data)
l[0:0] = [0]
l.append(l[-1] + 3)
diffs = [l[i] - l[i - 1] for i in range(1, len(l))]
#print(repr(diffs))
n1 = len([x for x in diffs if x == 1])
n3 = len([x for x in diffs if x == 3])
print(n1, n3, n1 * n3)