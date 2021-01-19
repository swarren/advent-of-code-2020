#!/usr/bin/env python3

player1 = []
player2 = []
with open("day22.txt") as f:
    # Player 1:
    f.readline()
    while True:
        l = f.readline().strip()
        if not l:
            break
        player1.append(int(l))
    # Player 1:
    f.readline()
    while True:
        l = f.readline().strip()
        if not l:
            break
        player2.append(int(l))

#i = 0
while len(player1) and len(player2):
    #i += 1
    #print(f'round {i}')
    card1 = player1[0]
    #print(f'  card1 {card1}')
    player1 = player1[1:]
    card2 = player2[0]
    #print(f'  card2 {card2}')
    player2 = player2[1:]
    cards = [card1, card2]
    cards.sort(reverse=True)
    #print(f'  cards {cards}')
    if card1 > card2:
        player1.extend(cards)
    else:
        player2.extend(cards)
    #print(f'  p1 {player1} p2 {player2}')
player1.extend(player2)
#print(player1)
l = len(player1)
#print(player1)
print(sum(map(lambda ix: (l - ix[0]) * ix[1], enumerate(player1))))
