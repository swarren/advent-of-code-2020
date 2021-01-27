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

long countTrees(std::vector<std::string> input, int dX, int dY) {
    long countTrees = 0;
    int xIndex = 0;
    for (auto itLine = input.begin(); itLine != input.end(); safe_advance(itLine, input.end(), dY)) {
        if ((*itLine)[xIndex % (*itLine).length()] == '#')
            countTrees++;
        xIndex += dX;
    }
    return countTrees;
}

int main(void) {
    auto input = readParseInput("../input/day3.txt");

    std::pair<long, long> slopes[] = {
        std::make_pair<long, long>(1, 1),
        std::make_pair<long, long>(3, 1),
        std::make_pair<long, long>(5, 1),
        std::make_pair<long, long>(7, 1),
        std::make_pair<long, long>(1, 2)
    };
    std::vector<long> counts;
    auto genCount = [&input = std::as_const(input)](std::pair<long, long> slope){ return countTrees(input, slope.first, slope.second); };
    std::transform(std::begin(slopes), std::end(slopes), std::back_inserter(counts), genCount);
    // It's very important to specify the accumulator type via one of the following methods
    // (explicit template parameters or explict long initial value, rather than bare "1" as the init value)
    // or the accumulator will be int by default, and the result will wrap...
    //long answer = std::accumulate<std::vector<long>::iterator, long>(counts.begin(), counts.end(), 1, std::multiplies<long>());
    long answer = std::accumulate(counts.begin(), counts.end(), long(1), std::multiplies<long>());
    std::cout << answer << std::endl;

    return 0;
}
