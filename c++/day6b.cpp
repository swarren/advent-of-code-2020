#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

    uint32_t accum = (1 << 27) - 1;
    for (const std::string &s : input) {
        if (!s.length()) {
            result += __builtin_popcount(accum);
            accum = (1 << 27) - 1;
            continue;
        }
        uint32_t accum2 = 0;
        for (char c : s)
            accum2 |= 1 << (c - 'a');
        accum &= accum2;
    }
    result += __builtin_popcount(accum);

    return result;
}

int main(void) {
    const auto input = readParseInput("../input/day6.txt");
    auto answer_ = answer(input);
    std::cout << answer_ << '\n';
    return 0;
}
