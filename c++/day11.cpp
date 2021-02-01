#include <fstream>
#include <iostream>
#include <map>
#include <string>

using Coord = std::pair<int, int>;
using Grid = std::map<Coord, bool>;

auto readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Grid input;

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        int colNum = 0;
        for (auto c : line) {
            if (c == 'L') {
                Coord coord(lineNum, colNum);
                input[coord] = false;
            }
            colNum++;
        }
        lineNum++;
    }
    file.close();

    return input;
}

int calcAdjacentFilled(const Coord coord, const Grid &grid) {
    int result = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if ((dy == 0) && (dx == 0))
                continue;
            Coord newCoord(coord.first + dy, coord.second + dx);
            auto it = grid.find(newCoord);
            if (it == grid.end())
                continue;
            bool isOccupied = (*it).second;
            if (isOccupied)
                result++;
        }
    }
    return result;
}

Grid iter(const Grid &grid) {
    Grid newGrid;

    for (auto seatInfo : grid) {
        Coord coord = seatInfo.first;
        bool isOccupied = seatInfo.second;
        int adjacentFilled = calcAdjacentFilled(coord, grid);
        bool newOccupied = isOccupied;
        if (isOccupied) {
            if (adjacentFilled >= 4)
                newOccupied = false;
        } else {
            if (adjacentFilled == 0)
                newOccupied = true;
        }
        newGrid[coord] = newOccupied;
    }

    return newGrid;
}

int answer(Grid grid) {
    while (true) {
        Grid newGrid = iter(grid);
        if (newGrid == grid) {
            int full = 0;
            for (auto seatInfo : grid) {
                bool isOccupied = seatInfo.second;
                if (isOccupied)
                    full++;
            }
            return full;
        }
        grid = newGrid;
    }
}

int main(void) {
    std::cout << answer(readParseInput("../input/day11.txt")) << '\n';
    return 0;
}
