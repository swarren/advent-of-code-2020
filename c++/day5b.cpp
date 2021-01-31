#include <fstream>
#include <iostream>
#include <string>
#include <vector>

auto readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::vector<int> result;

    std::string line;
    while (std::getline(file, line)) {
        int v = 0;
        for (auto c: line) {
            v <<= 1;
            if ((c == 'B') || (c == 'R'))
                v |= 1;
        }
        result.push_back(v);
    }
    file.close();

    return result;
}

int answer(std::vector<int> &input) {
    sort(input.begin(), input.end());
    auto prev = input.front() - 1;
    for (auto v : input) {
        if (v != prev + 1)
            return prev + 1;
        prev = v;
    }
    return input.back();
}

int main(void) {
    auto input = readParseInput("../input/day5.txt");
    auto answer_ = answer(input);
    std::cout << answer_ << '\n';
    return 0;
}
