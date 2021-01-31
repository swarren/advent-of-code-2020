#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
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

template <typename Iterator>
void safe_advance(Iterator& it, Iterator const& end, size_t n) {
    for (size_t i = 0; i != n && it != end; ++i)
        ++it;
}

constexpr int countTrees(const std::vector<std::string> &input, int dX, int dY) {
    int countTrees = 0;
    int xIndex = 0;
    for (auto itLine = input.cbegin(); itLine != input.cend(); safe_advance(itLine, input.cend(), dY)) {
        if ((*itLine)[xIndex % (*itLine).length()] == '#')
            countTrees++;
        xIndex += dX;
    }
    return countTrees;
}

int main(void) {
    const auto input = readParseInput("../input/day3.txt");

    const std::pair<int, int> slopes[] = {
        std::make_pair<int, int>(1, 1),
        std::make_pair<int, int>(3, 1),
        std::make_pair<int, int>(5, 1),
        std::make_pair<int, int>(7, 1),
        std::make_pair<int, int>(1, 2)
    };
    std::vector<int> counts;
    auto genCount = [&input = std::as_const(input)](const auto &slope){ return countTrees(input, slope.first, slope.second); };
    std::transform(std::cbegin(slopes), std::cend(slopes), std::back_inserter(counts), genCount);
    const auto answer = std::accumulate(counts.cbegin(), counts.cend(), long(1), std::multiplies<>());
    std::cout << answer << std::endl;

    return 0;
}