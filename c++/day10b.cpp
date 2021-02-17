#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Input = std::vector<long>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    std::string line;
    while (std::getline(file, line))
        input.push_back(std::atoi(line.c_str()));
    file.close();

    return input;
}

long combos(int l) {
    if (l < 2)
        return 1;
    int ret = 1;
    for (int i = 0; i < l - 1; ++i)
        ret += combos(l - i - 2);
    for (int i = 0; i < l - 2; ++i)
        ret += combos(l - i - 3);
    return ret;
}

long answer(Input input) {
    std::ranges::sort(input);

    long combinations = 1;
    long diff1Count = 0;
    long prev = 0;
    for (long v : input) {
        if (v - prev == 1) {
            diff1Count++;
        } else if (v - prev == 3) {
            combinations *= combos(diff1Count);
            diff1Count = 0;
        }
        prev = v;
    }
    combinations *= combos(diff1Count);
    return combinations;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day10.txt")) << '\n';
    return 0;
}
