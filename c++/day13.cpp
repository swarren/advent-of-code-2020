#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using Input = std::pair<int, std::vector<int>>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;

    std::getline(file, line);
    int arrivalTime = std::atoi(&line[0]);

    std::getline(file, line);
    std::vector<int> periods;
    std::string::size_type parsePos = 0;
    while (true) {
        auto commaPos = line.find(',', parsePos);
        std::string periodStr = line.substr(parsePos, commaPos - parsePos);
        if (periodStr[0] != 'x') {
            int period = atoi(periodStr.c_str());
            periods.push_back(period);
        }
        if (commaPos == std::string::npos)
            break;
        parsePos = commaPos + 1;
    }
    file.close();

    return Input(arrivalTime, periods);
}

int answer(const Input &input) {
    const int arrivalTime = input.first;
    const std::vector<int> &periods = input.second;

    // std::transform, std::accumulate could work here, but are just too
    // verbose and painful.
    int earliestWhen = 0;
    int earliestPeriod = 0;
    for (int period : periods) {
        int remainder = arrivalTime % period;
        int when = arrivalTime;
        if (remainder)
            when += period - remainder;
        if (!earliestWhen || when < earliestWhen) {
            earliestWhen = when;
            earliestPeriod = period;
        }
    }

    return (earliestWhen - arrivalTime) * earliestPeriod;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day13.txt")) << '\n';
    return 0;
}
