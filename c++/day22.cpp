#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <set>
#include <string>

using Card = int;
using Hand = std::deque<Card>;
struct Hands {
    Hand player1;
    Hand player2;
};

Hands readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Hands hands;

    std::string line;
    // Player 1:
    std::getline(file, line);
    while (std::getline(file, line)) {
        if (!line.length())
            break;
        hands.player1.emplace_back(atoi(line.c_str()));
    }
    // Player 2:
    std::getline(file, line);
    while (std::getline(file, line))
        hands.player2.emplace_back(atoi(line.c_str()));
    file.close();

    return hands;
}

bool playGame(Hands &hands) {
    while (true) {
        // Player 1 wins
        if (!hands.player2.size())
            return true;
        // Player 2 wins
        if (!hands.player1.size())
            return false;
        Card card1 = hands.player1.front();
        hands.player1.pop_front();
        Card card2 = hands.player2.front();
        hands.player2.pop_front();
        if (card1 > card2) {
            hands.player1.push_back(card1);
            hands.player1.push_back(card2);
        } else {
            hands.player2.push_back(card2);
            hands.player2.push_back(card1);
        }
    }
}

int answer(Hands hands) {
    bool player1Won = playGame(hands);
    Hand &winningHand = player1Won ? hands.player1 : hands.player2;
    int len = winningHand.size();
    int answer = 0;
    for (const auto &card : winningHand)
        answer += len-- * card;
    return answer;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day22.txt")) << '\n';
    return 0;
}
