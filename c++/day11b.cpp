#include <fstream>
#include <iostream>
#include <map>
#include <string>

using Coord = std::pair<int, int>;
using Grid = std::map<Coord, bool>;
using Input = std::tuple<Grid, int, int>;

auto readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Grid grid;

    std::string line;
    int lineNum = 0;
    int colNum;
    while (std::getline(file, line)) {
        colNum = 0;
        for (auto c : line) {
            if (c == 'L') {
                Coord coord(lineNum, colNum);
                grid[coord] = false;
            }
            colNum++;
        }
        lineNum++;
    }
    file.close();

    return Input(grid, lineNum, colNum);
}

int calcAdjacentFilled(const Coord coord, const Grid &grid, int maxRayLen) {
    int result = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if ((dy == 0) && (dx == 0))
                continue;
            for (int mult = 1; mult <= maxRayLen; ++mult) {
                Coord newCoord(coord.first + (mult * dy), coord.second + (mult * dx));
                auto it = grid.find(newCoord);
                if (it == grid.end())
                    continue;
                bool isOccupied = (*it).second;
                if (isOccupied)
                    result++;
                break;
            }
        }
    }
    return result;
}

Grid iter(const Grid &grid, int maxRayLen) {
    Grid newGrid;

    for (auto seatInfo : grid) {
        Coord coord = seatInfo.first;
        bool isOccupied = seatInfo.second;
        int adjacentFilled = calcAdjacentFilled(coord, grid, maxRayLen);
        bool newOccupied = isOccupied;
        if (isOccupied) {
            if (adjacentFilled >= 5)
                newOccupied = false;
        } else {
            if (adjacentFilled == 0)
                newOccupied = true;
        }
        newGrid[coord] = newOccupied;
    }

    return newGrid;
}

int answer(const Input &input) {
    Grid grid = std::get<0>(input);
    int height = std::get<1>(input);
    int width = std::get<2>(input);
    int maxRayLen = std::max(height, width);

    while (true) {
        Grid newGrid = iter(grid, maxRayLen);
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
    const Input &input = readParseInput("../input/day11.txt");
    std::cout << answer(input) << '\n';
    return 0;
}
