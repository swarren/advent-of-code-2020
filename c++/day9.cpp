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

bool isSum(const Input &input, int pos) {
    const int histLen = 25;

    for (int firstPos = pos - histLen; firstPos <= pos - 2; ++firstPos) {
        for (int secondPos = firstPos + 1; secondPos <= pos - 1; ++secondPos) {
            int firstVal = input[firstPos];
            int secondVal = input[secondPos];
            if (firstVal == secondVal)
                continue;
            int sum = firstVal + secondVal;
            if (sum != input[pos])
                continue;
            return true;
        }
    }
    return false;
}

int answer(const Input &input) {
    for (int pos = 25; pos < input.size(); ++pos) {
        if (!isSum(input, pos))
            return input[pos];
    }
    return -1;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day9.txt")) << '\n';
    return 0;
}
