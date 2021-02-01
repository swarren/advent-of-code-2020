#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Input = std::vector<int>;

auto readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    std::string line;
    while (std::getline(file, line))
        input.push_back(std::atoi(line.c_str()));
    file.close();

    return input;
}

int answer(Input &input) {
    std::sort(input.begin(), input.end());

    int diff1Count = 0;
    int diff3Count = 0;
    int prev = 0;
    for (int v : input) {
        if (v - prev == 1)
            diff1Count++;
        else if (v - prev == 3)
            diff3Count++;
        prev = v;
    }
    diff3Count++;
    return diff1Count * diff3Count;
}

int main(void) {
    Input input = readParseInput("../input/day10.txt");
    std::cout << answer(input) << '\n';
    return 0;
}
