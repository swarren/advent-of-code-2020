#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
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
    int countTrees = 0;
    int xIndex = 0;
    for (auto itLine = input.begin(); itLine != input.end(); ++itLine) {
        if ((*itLine)[xIndex % (*itLine).length()] == '#')
            countTrees++;
        xIndex += 3;
    }
    return countTrees;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day3.txt")) << '\n';
    return 0;
}
