#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

using Input = std::vector<std::string>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    std::string line;
    while (std::getline(file, line))
        input.push_back(line);
    file.close();

    return input;
}

int answer(const Input &input) {
    return std::ranges::count_if(
        input,
        [xIndex=-3] (const std::string &line) mutable {
            xIndex += 3;
            return line[xIndex % line.length()] == '#';
        }
    );
}

int main(void) {
    std::cout << answer(readParseInput("../input/day3.txt")) << '\n';
    return 0;
}
