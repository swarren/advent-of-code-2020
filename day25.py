#!/usr/bin/env python3

# example
#data = [5764801, 17807724]
# my data
data = [10441485, 1004920]

n = 1
iters = 0
while True:
    n *= 7
    n %= 20201227
    iters += 1
    if n == data[0]:
        break
#print(iters)

n = 1
for i in range(iters):
    n *= data[1]
    n %= 20201227
print(n)
