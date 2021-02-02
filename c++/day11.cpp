#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>

using Coord = std::pair<int, int>;
using Grid = std::map<Coord, bool>;

Grid *readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Grid *grid = new Grid();

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        int colNum = 0;
        for (auto c : line) {
            if (c == 'L') {
                Coord coord(lineNum, colNum);
                (*grid)[coord] = false;
            }
            colNum++;
        }
        lineNum++;
    }
    file.close();

    return grid;
}

int calcAdjacentFilled(const Coord coord, const Grid *grid) {
    int result = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if ((dy == 0) && (dx == 0))
                continue;
            Coord newCoord(coord.first + dy, coord.second + dx);
            auto it = grid->find(newCoord);
            if (it == grid->end())
                continue;
            bool isOccupied = (*it).second;
            if (isOccupied)
                result++;
        }
    }
    return result;
}

Grid *iter(const Grid *grid) {
    Grid *newGrid = new Grid();

    for (auto seatInfo : *grid) {
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
        (*newGrid)[coord] = newOccupied;
    }

    return newGrid;
}

int answer(const Grid *gridInit) {
    std::shared_ptr<const Grid> grid(gridInit);

    while (true) {
        std::shared_ptr<const Grid> newGrid(iter(grid.get()));
        if (*(newGrid.get()) == *(grid.get())) {
            int full = 0;
            for (auto seatInfo : *(grid.get())) {
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
