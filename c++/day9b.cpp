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

int answer(Input &input) {
    int invalidNumber = -1;
    for (int pos = 25; pos < input.size(); ++pos) {
        if (!isSum(input, pos)) {
            invalidNumber = input[pos];
            break;
        }
    }

    const int minLength = 2;
    for (int startPos = 0; startPos <= input.size() - minLength; ++startPos) {
        for (int afterPos = startPos + minLength; afterPos < input.size(); ++afterPos) {
            int sum = 0;
            for (int i = startPos; i < afterPos; ++i)
                sum += input[i];
            if (sum == invalidNumber) {
                std::sort(&input[startPos], &input[afterPos]);
                return input[startPos] + input[afterPos - 1];
            }
        }
    }

    return -1;
}

int main(void) {
    Input input = readParseInput("../input/day9.txt");
    std::cout << answer(input) << '\n';
    return 0;
}
