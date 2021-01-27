#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::vector<std::string> result;

    std::string line;
    while (std::getline(file, line))
        result.push_back(line);
    file.close();

    return result;
}

int main(void) {
    auto input = readParseInput("../input/day3.txt");

    int countTrees = 0;
    int xIndex = 0;
    for (auto itLine = input.begin(); itLine != input.end(); ++itLine) {
        if ((*itLine)[xIndex % (*itLine).length()] == '#')
            countTrees++;
        xIndex += 3;
    }
    std::cout << countTrees << std::endl;

    return 0;
}
