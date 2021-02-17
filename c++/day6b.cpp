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
    accum.set();
    for (const std::string &s : input) {
        if (!s.length()) {
            result += accum.count();
            accum.set();
            continue;
        }
        Bits accum2;
        for (char c : s)
            accum2[c - 'a'] = true;
        accum &= accum2;
    }
    result += accum.count();

    return result;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day6.txt")) << '\n';
    return 0;
}
