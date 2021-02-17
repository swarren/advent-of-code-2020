#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Bits = std::bitset<26>;
using Input = std::vector<std::string>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    std::string line;
    while (std::getline(file, line)) {
        input.push_back(line);
    }
    file.close();

    return input;
}

int answer(const Input &input) {
    int result = 0;

    Bits accum;
    for (const std::string &s : input) {
        if (!s.length()) {
            result += accum.count();
            accum.reset();
            continue;
        }
        for (char c : s)
            accum[c - 'a'] = true;
    }
    result += accum.count();

    return result;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day6.txt")) << '\n';
    return 0;
}
