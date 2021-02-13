#include <array>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using Input = std::vector<std::string>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    std::string line;
    while (std::getline(file, line))
        input.emplace_back(line);

    return input;
}

using Coord = std::array<int, 2>;
using Floor = std::set<Coord>;

int answer(const Input &input) {
    Floor floor{};

    for (const auto &moves : input) {
        Coord coord{};
        int ew = 2;
        for (char c : moves) {
            switch (c) {
            case 'n':
                coord[1] += 2;
                ew = 1;
                break;
            case 's':
                coord[1] -= 2;
                ew = 1;
                break;
            case 'e':
                coord[0] += ew;
                ew = 2;
                break;
            case 'w':
                coord[0] -= ew;
                ew = 2;
                break;
            }
        }
        if (floor.contains(coord))
            floor.erase(coord);
        else
            floor.insert(coord);
    }

    return floor.size();
}

int main(void) {
    std::cout << answer(readParseInput("../input/day24.txt")) << '\n';
    return 0;
}
