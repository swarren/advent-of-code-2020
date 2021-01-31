#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using ChildBag = std::pair<int, std::string>;
using BagContent = std::vector<ChildBag>;
using Input = std::map<std::string, BagContent>;

auto readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input result;

    std::string line;
    while (std::getline(file, line)) {
        // mirrored brown bags contain 1 pale teal bag, 3 muted gray bags, 3 dark bronze bags.
        // shiny violet bags contain 3 vibrant crimson bags.
        // dotted black bags contain no other bags.

        auto spacePos = line.find(' ', 0);
        spacePos = line.find(' ', spacePos + 1);
        std::string thisBagName = line.substr(0, spacePos);

        spacePos = line.find(' ', spacePos + 1);
        spacePos = line.find(' ', spacePos + 1);

        BagContent bagContent;

        auto parsePos = spacePos + 1;
        if (line[parsePos] != 'n') { // no other bags
            while (true) {
                spacePos = line.find(' ', parsePos);
                std::string countStr = line.substr(parsePos, spacePos - parsePos);
                std::istringstream iss(countStr);
                int count;
                iss >> count;
                parsePos = spacePos + 1;

                spacePos = line.find(' ', parsePos);
                spacePos = line.find(' ', spacePos + 1);
                std::string childBagName = line.substr(parsePos, spacePos - parsePos);
                bagContent.push_back(ChildBag(count, childBagName));
                spacePos = line.find(' ', spacePos + 1);
                if (spacePos == std::string::npos)
                    break;
                parsePos = spacePos + 1;
            }
        }

        result[thisBagName] = bagContent;
    }
    file.close();

    return result;
}

int answer(const Input &input) {
    std::map<std::string, bool> canBeGold;
    std::set<std::string> visited;

    std::function<bool(const std::string &)> calcCanBeGold = [&](const std::string &bagName) {
        if (visited.find(bagName) != visited.end()) {
            return canBeGold[bagName];
        }

        bool result = false;
        if (bagName == "shiny gold") {
            result = true;
        } else {
            auto bagIt = input.find(bagName);
            auto bagContent = (*bagIt).second;

            for (auto cb : bagContent) {
                auto childBagName = cb.second;

                if (calcCanBeGold(childBagName)) {
                    result = true;
                    break;
                }
            }
        }

        visited.insert(bagName);
        canBeGold[bagName] = result;
        return result;
    };

    int count = 0;
    for (auto b : input) {
        auto bagName = b.first;
        if (calcCanBeGold(bagName))
            count++;
    }
    return count - 1;
}

int main(void) {
    const auto input = readParseInput("../input/day7.txt");
    const auto answer_ = answer(input);
    std::cout << answer_ << '\n';

    return 0;
}
