#include <algorithm>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <string>

using Card = unsigned int;
using Hand = std::deque<Card>;
struct Hands {
    Hand player1;
    Hand player2;

    auto operator<=>(const Hands&) const = default;
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
    std::set<Hands> seenStates;
    while (true) {
        // Player 1 wins
        if (!hands.player2.size())
            return true;
        // Player 2 wins
        if (!hands.player1.size())
            return false;
        auto [it, newState] = seenStates.insert(hands);
        if (!newState)
            return true;
        Card card1 = hands.player1.front();
        hands.player1.pop_front();
        auto len1 = hands.player1.size();
        Card card2 = hands.player2.front();
        hands.player2.pop_front();
        auto len2 = hands.player2.size();
        bool player1Won;
        if ((len1 >= card1) && (len2 >= card2)) {
            Hands subHands;
            std::copy(hands.player1.begin(), hands.player1.begin() + card1, std::inserter(subHands.player1, subHands.player1.begin()));
            std::copy(hands.player2.begin(), hands.player2.begin() + card2, std::inserter(subHands.player2, subHands.player2.begin()));
            player1Won = playGame(subHands);
        } else {
            player1Won = card1 > card2;
        }
        if (player1Won) {
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
