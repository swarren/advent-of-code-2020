#!/usr/bin/env python3

import data22 as data

game_id_ctr = 1

def play_game(p1, p2, game_id):
    global game_id_ctr

    seen_configs = set()
    round_num = 0
    while len(p1) and len(p2):
        round_num += 1
        #print(f'game {game_id} round {round_num}')
        #print(f'  p1 {p1}')
        #print(f'  p2 {p2}')

        config = tuple(p1 + [-1] + p2)
        #print(f'  config {config}')
        if config in seen_configs:
            #print(f'  game {game_id} winner 1; config alredy seen: {config}')
            return True, p1
        seen_configs.add(config)

        card1 = p1[0]
        #print(f'  card1 {card1}')
        p1 = p1[1:]
        card2 = p2[0]
        #print(f'  card2 {card2}')
        p2 = p2[1:]

        if len(p1) >= card1 and len(p2) >= card2:
            game_id_ctr += 1
            #print(f'  recurse -> game {game_id_ctr}')
            winner1, _ = play_game(p1[:card1], p2[:card2], game_id_ctr)
            #print(f'game {game_id} round {round_num} (return)')
        else:
            winner1 = card1 > card2

        #print(f'  cards {cards}')
        if winner1:
            #print(f'  round winner 1')
            p1.extend([card1, card2])
        else:
            #print(f'  round winner 2')
            p2.extend([card2, card1])
        #print(f'  p1 {p1}')
        #print(f'  p2 {p2}')

    if len(p1):
        #print(f'  game {game_id} winner 1')
        return True, p1
    else:
        #print(f'  game {game_id} winner 2')
        return False, p2

_, cards_winner = play_game(data.player1[:], data.player2[:], game_id_ctr)
#print(cards_winner)
l = len(cards_winner)
print(sum(map(lambda ix: (l - ix[0]) * ix[1], enumerate(cards_winner))))
