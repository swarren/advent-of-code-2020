#!/usr/bin/env python3

import data22 as data

#i = 0
while len(data.player1) and len(data.player2):
    #i += 1
    #print(f'round {i}')
    card1 = data.player1[0]
    #print(f'  card1 {card1}')
    data.player1 = data.player1[1:]
    card2 = data.player2[0]
    #print(f'  card2 {card2}')
    data.player2 = data.player2[1:]
    cards = [card1, card2]
    cards.sort(reverse=True)
    #print(f'  cards {cards}')
    if card1 > card2:
        data.player1.extend(cards)
    else:
        data.player2.extend(cards)
    #print(f'  p1 {data.player1} p2 {data.player2}')
data.player1.extend(data.player2)
#print(data.player1)
l = len(data.player1)
#print(data.player1)
print(sum(map(lambda ix: (l - ix[0]) * ix[1], enumerate(data.player1))))
