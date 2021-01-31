#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Bits = std::bitset<26>;

auto readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::vector<std::string> result;

    std::string line;
    while (std::getline(file, line)) {
        result.push_back(line);
    }
    file.close();

    return result;
}

int answer(const std::vector<std::string> &input) {
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
    const auto input = readParseInput("../input/day6.txt");
    auto answer_ = answer(input);
    std::cout << answer_ << '\n';
    return 0;
}
