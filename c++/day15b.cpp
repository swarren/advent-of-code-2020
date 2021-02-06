#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using Input = std::vector<int>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    std::string line;
    std::getline(file, line);
    std::string::size_type parsePos = 0;
    while (parsePos != std::string::npos) {
        input.emplace_back(atoi(&line[parsePos]));
        parsePos = line.find(',', parsePos);
        if (parsePos != std::string::npos)
            parsePos++;
    }
    file.close();

    return input;
}

int answer(const Input &input) {
    std::map<int, int> lastSaid;

    auto sayWord = [&](int val, int whenSaid) {
        auto it = lastSaid.find(val);
        int nextSay;
        if (it == lastSaid.end())
            nextSay = 0;
        else
            nextSay = whenSaid - it->second;
        lastSaid[val] = whenSaid;
        return nextSay;
    };
    int nextSay = 0;
    int whenSaid = 1;
    for (auto toSay : input)
        nextSay = sayWord(toSay, whenSaid++);
    while (whenSaid < 30000000) {
        nextSay = sayWord(nextSay, whenSaid++);
    }

    return nextSay;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day15.txt")) << '\n';
    return 0;
}
