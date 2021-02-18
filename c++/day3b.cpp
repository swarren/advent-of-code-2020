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

template <typename Iterator>
void safe_advance(Iterator& it, Iterator const& end, size_t n) {
    for (size_t i = 0; i != n && it != end; ++i)
        ++it;
}

constexpr int countTrees(const std::vector<std::string> &input, const int dX, const int dY) {
    return std::ranges::count_if(
        input,
        [y=-1, xIndex=-dX, &dX, &dY] (const std::string &line) mutable {
            y++;
            if (y % dY)
                return false;
            xIndex += dX;
            return line[xIndex % line.length()] == '#';
        }
    );
}

long answer(const Input &input) {
    const std::pair<int, int> slopes[] = {
        std::make_pair(1, 1),
        std::make_pair(3, 1),
        std::make_pair(5, 1),
        std::make_pair(7, 1),
        std::make_pair(1, 2)
    };
    long answer = 1;
    for (const auto &[dX, dY] : slopes)
        answer *= countTrees(input, dX, dY);
    return answer;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day3.txt")) << '\n';
    return 0;
}
