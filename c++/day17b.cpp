#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <set>
#include <string>

static const bool debug = false;

using Coord = std::array<int, 4>;
using Grid = std::set<Coord>;

Grid *readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Grid *grid = new Grid();

    std::string line;
    int y = 0;
    while (std::getline(file, line)) {
        int x = 0;
        for (auto c : line) {
            if (c == '#') {
                Coord coord = {x, y, 0, 0};
                grid->insert(coord);
            }
            x++;
        }
        y++;
    }
    file.close();

    return grid;
}

std::ostream& operator<<(std::ostream& os, const Coord& coord)
{
    os << coord[0] << ',' << coord[1] << ',' << coord[2] << ',' << coord[3];
    return os;
}

std::ostream& operator<<(std::ostream& os, const Grid& grid)
{
    int axisMin[4];
    int axisMax[4];

    for (int axis = 0; axis < 4; ++axis) {
        axisMin[axis] = std::numeric_limits<int>::max();
        axisMax[axis] = std::numeric_limits<int>::min();
    }
    
    for (const Coord &coord : grid) {
        for (int axis = 0; axis < 4; ++axis) {
            axisMin[axis] = std::min(axisMin[axis], coord[axis]);
            axisMax[axis] = std::max(axisMax[axis], coord[axis]);
        }
    }

    std::cout <<  'x' << axisMin[0] << ".." << axisMax[0]
              << " y" << axisMin[1] << ".." << axisMax[1]
              << " z" << axisMin[2] << ".." << axisMax[2]
              << " w" << axisMin[3] << ".." << axisMax[3] << '\n';

    for (int w = axisMin[3]; w <= axisMax[3]; ++w) {
        for (int z = axisMin[2]; z <= axisMax[2]; ++z) {
            std::cout << "z=" << z << ", w=" << w << '\n';
            for (int y = axisMin[1]; y <= axisMax[1]; ++y) {
                for (int x = axisMin[0]; x <= axisMax[0]; ++x) {
                    Coord coord = {x, y, z};
                    bool isOccupied = grid.find(coord) != grid.cend();
                    std::cout << (isOccupied ? '#' : '.');
                }
                std::cout << '\n';
            }
            std::cout << '\n';
        }
    }

    return os;
}

int calcAdjacentFilled(const Coord coord, const Grid *grid, const std::set<Coord> &visited, std::set<Coord> &toVisit) {
    int result = 0;
    bool isOccupied = grid->find(coord) != grid->cend();
    for (int dw = -1; dw <= 1; ++dw) {
        for (int dz = -1; dz <= 1; ++dz) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if ((dw == 0) && (dz == 0) && (dy == 0) && (dx == 0))
                        continue;
                    Coord newCoord = {coord[0] + dx, coord[1] + dy, coord[2] + dz, coord[3] + dw};
                    if (debug) std::cout << "    sibling coord: " << newCoord << '\n';
                    if (isOccupied && (visited.find(newCoord) == visited.cend())) {
                        if (debug) std::cout << "      adding toVisit\n";
                        toVisit.insert(newCoord);
                    }
                    if (grid->find(newCoord) != grid->cend()) {
                        result++;
                        if (debug) std::cout << "      increasing adjacentFilled; now " << result << '\n';
                    }
                }
            }
        }
    }
    return result;
}

Grid *nextGrid(const Grid *grid) {
    Grid *newGrid = new Grid();
    std::set<Coord> toVisit;
    std::set<Coord> visited;

    for (auto &coord : *grid)
        toVisit.insert(coord);

    while (true) {
        auto it = toVisit.begin();
        if (it == toVisit.end())
            break;
        const Coord coord = *it;

        if (debug) std::cout << "  visiting " << coord << '\n';
        toVisit.erase(coord);
        visited.insert(coord);

        int adjacentFilled = calcAdjacentFilled(coord, grid, visited, toVisit);
        bool isOccupied = grid->find(coord) != grid->cend();
        bool newOccupied;
        if (isOccupied)
            newOccupied = (adjacentFilled == 2) || (adjacentFilled == 3);
        else
            newOccupied = adjacentFilled == 3;
        if (debug) std::cout << "    " << isOccupied << " -> " << newOccupied << " (" << adjacentFilled << ")\n";
        if (newOccupied)
            newGrid->insert(coord);
    }

    return newGrid;
}

int answer(const Grid *gridInit) {
    std::shared_ptr<const Grid> grid(gridInit);

    if (debug) std::cout << "Before any cycles:\n\n";
    if (debug) std::cout << *grid.get();

    for (auto iter = 0; iter < 6; ++iter) {
        std::shared_ptr<const Grid> newGrid(nextGrid(grid.get()));
        grid = newGrid;
        if (debug) std::cout << "After " << (iter + 1) << " cycles:\n\n";
        if (debug) std::cout << *grid.get();
    }

    return grid->size();
}

int main(void) {
    std::cout << answer(readParseInput("../input/day17.txt")) << '\n';
    return 0;
}
