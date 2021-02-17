#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Input = std::vector<int>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    std::string line;
    while (std::getline(file, line)) {
        int v = 0;
        for (auto c: line) {
            v <<= 1;
            if ((c == 'B') || (c == 'R'))
                v |= 1;
        }
        input.push_back(v);
    }
    file.close();

    return input;
}

int answer(Input input) {
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
    std::cout << answer(readParseInput("../input/day5.txt")) << '\n';
    return 0;
}
