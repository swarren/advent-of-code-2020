#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using Input = std::vector<std::pair<long, long>>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;
    std::string line;

    std::getline(file, line);
    std::getline(file, line);
    std::string::size_type parsePos = 0;
    long offset = 0;
    while (true) {
        auto commaPos = line.find(',', parsePos);
        std::string periodStr = line.substr(parsePos, commaPos - parsePos);
        if (periodStr[0] != 'x') {
            long period = atol(periodStr.c_str());
            input.push_back(std::make_pair(offset, period));
        }
        if (commaPos == std::string::npos)
            break;
        parsePos = commaPos + 1;
        offset++;
    }
    file.close();

    return input;
}

long answer(const Input &input) {
    for (long n = 0; ; ++n) {
        long t = (937 * (569 * n + 14)) - 44;
        bool isMatch = true;
        for (auto info : input) {
            long offset = info.first;
            long period = info.second;
            long busTime = t + offset;
            if (busTime % period) {
                isMatch = false;
                break;
            }
        }
        if (isMatch)
            return t;
    }
}

int main(void) {
    std::cout << answer(readParseInput("../input/day13.txt")) << '\n';
    return 0;
}
