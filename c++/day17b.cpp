#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <set>
#include <string>

static const bool printGrids = false;
static const bool printCalcs = false;
static const std::size_t AXIS_COUNT = 4;

using Coord = std::array<int, AXIS_COUNT>;
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
                Coord coord;
                coord[0] = x;
                coord[1] = y;
                grid->insert(coord);
            }
            x++;
        }
        y++;
    }
    file.close();

    return grid;
}

template <typename T, std::size_t N, std::size_t IX>
std::ostream& arrayToOstream(std::ostream& os, const std::array<T, N> &coord)
{
    os << coord[IX];
    if constexpr(IX == N - 1) {
        return os;
    } else {
        std::cout << ',';
        return arrayToOstream<T, N, IX + 1>(os, coord);
    }
}

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const std::array<T, N> &coord)
{
    return arrayToOstream<T, N, 0>(os, coord);
}

char axisName(std::size_t axis) {
    if (axis < 3)
        return 'x' + axis;
    else if (axis == 3)
        return 'w';
    else
        return 'a' + axis;
}

struct GridDumpAxisInfo {
    int axisMin;
    int axisMax;
    int axisVal;
};

template <std::size_t N, std::size_t AXIS>
std::ostream& gridToOstream(std::ostream& os, const Grid& grid, GridDumpAxisInfo *gridDumpAxisInfo) {
    if constexpr(AXIS == 1) {
        for (std::size_t axis = 2; axis < N; ++axis) {
            if (axis != 2)
                std::cout << ", ";
            std::cout << axisName(axis) << '=' << gridDumpAxisInfo[axis].axisVal;
        }
        std::cout << '\n';
        for (int y = gridDumpAxisInfo[1].axisMin; y <= gridDumpAxisInfo[1].axisMax; ++y) {
            for (int x = gridDumpAxisInfo[0].axisMin; x <= gridDumpAxisInfo[0].axisMax; ++x) {
                Coord coord;
                coord[0] = x;
                coord[1] = y;
                for (std::size_t axis = 2; axis < N; ++axis)
                    coord[axis] = gridDumpAxisInfo[axis].axisVal;
                bool isOccupied = grid.find(coord) != grid.cend();
                std::cout << (isOccupied ? '#' : '.');
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    } else {
        for (int i = gridDumpAxisInfo[AXIS].axisMin; i <= gridDumpAxisInfo[AXIS].axisMax; ++i) {
            gridDumpAxisInfo[AXIS].axisVal = i;
            gridToOstream<N, AXIS - 1>(os, grid, gridDumpAxisInfo);
        }
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const Grid& grid)
{
    GridDumpAxisInfo gridDumpAxisInfo[AXIS_COUNT];

    for (int axis = 0; axis < AXIS_COUNT; ++axis) {
        gridDumpAxisInfo[axis].axisMin = std::numeric_limits<int>::max();
        gridDumpAxisInfo[axis].axisMax = std::numeric_limits<int>::min();
    }
    
    for (const Coord &coord : grid) {
        for (int axis = 0; axis < AXIS_COUNT; ++axis) {
            gridDumpAxisInfo[axis].axisMin = std::min(gridDumpAxisInfo[axis].axisMin, coord[axis]);
            gridDumpAxisInfo[axis].axisMax = std::max(gridDumpAxisInfo[axis].axisMax, coord[axis]);
        }
    }

    for (int axis = 0; axis < AXIS_COUNT; ++axis) {
        if (axis)
            std::cout << ' ';
        std::cout << axisName(axis) << ' ' << gridDumpAxisInfo[axis].axisMin << ".." << gridDumpAxisInfo[axis].axisMax;
    }
    std::cout << '\n';
    std::cout << '\n';

    return gridToOstream<AXIS_COUNT, AXIS_COUNT - 1>(os, grid, gridDumpAxisInfo);
}

template <std::size_t N, std::size_t AXIS>
int calcAdjacentFilled(const Coord coord, const Grid *grid, const std::set<Coord> &visited, std::set<Coord> &toVisit, bool isOccupied, Coord &newCoord) {
    if constexpr(AXIS == 1) {
        int result = 0;
        bool othersAllEqual = true;
        for (std::size_t axis = 2; axis < N; ++axis) {
            if (newCoord[axis] != coord[axis]) {
                othersAllEqual = false;
                break;
            }
        }
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (othersAllEqual && (dy == 0) && (dx == 0))
                    continue;
                newCoord[1] = coord[1] + dy;
                newCoord[0] = coord[0] + dx;
                if (printCalcs) std::cout << "    sibling coord: " << newCoord << '\n';
                if (isOccupied && (visited.find(newCoord) == visited.cend())) {
                    if (printCalcs) std::cout << "      adding toVisit\n";
                    toVisit.insert(newCoord);
                }
                if (grid->find(newCoord) != grid->cend()) {
                    result++;
                    if (printCalcs) std::cout << "      increasing adjacentFilled; now " << result << '\n';
                }
            }
        }
        return result;
    } else {
        int result = 0;
        for (int d = -1; d <= 1; ++d) {
            newCoord[AXIS] = coord[AXIS] + d;
            result += calcAdjacentFilled<N, AXIS - 1>(coord, grid, visited, toVisit, isOccupied, newCoord);
        }
        return result;
    }
}

int calcAdjacentFilled(const Coord coord, const Grid *grid, const std::set<Coord> &visited, std::set<Coord> &toVisit) {
    Coord newCoord;

    bool isOccupied = grid->find(coord) != grid->cend();
    return calcAdjacentFilled<AXIS_COUNT, AXIS_COUNT - 1>(coord, grid, visited, toVisit, isOccupied, newCoord);
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

        if (printCalcs) std::cout << "  visiting " << coord << '\n';
        toVisit.erase(coord);
        visited.insert(coord);

        int adjacentFilled = calcAdjacentFilled(coord, grid, visited, toVisit);
        bool isOccupied = grid->find(coord) != grid->cend();
        bool newOccupied;
        if (isOccupied)
            newOccupied = (adjacentFilled == 2) || (adjacentFilled == 3);
        else
            newOccupied = adjacentFilled == 3;
        if (printCalcs) std::cout << "    " << isOccupied << " -> " << newOccupied << " (" << adjacentFilled << ")\n";
        if (newOccupied)
            newGrid->insert(coord);
    }

    return newGrid;
}

int answer(const Grid *gridInit) {
    std::shared_ptr<const Grid> grid(gridInit);

    if (printGrids) std::cout << "Before any cycles:\n\n";
    if (printGrids) std::cout << *grid.get();

    for (auto iter = 0; iter < 6; ++iter) {
        std::shared_ptr<const Grid> newGrid(nextGrid(grid.get()));
        grid = newGrid;
        if (printGrids) {
            std::cout << "After " << (iter + 1) << " cycle";
            if (iter != 0)
                std::cout << 's';
            std::cout << ":\n\n";
            std::cout << *grid.get();
        }
    }

    return grid->size();
}

int main(void) {
    std::cout << answer(readParseInput("../input/day17.txt")) << '\n';
    return 0;
}
