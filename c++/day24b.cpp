#include <array>
#include <fstream>
#include <iostream>
#include <memory>
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

int calcAdjacentFilled(const Coord coord, const Floor &floor, const std::set<Coord> &visited, std::set<Coord> &toVisit) {
    static const Coord offsets[] = {
        { 2,  0},
        { 1,  2},
        {-1,  2},
        {-2,  0},
        {-1, -2},
        { 1, -2},
    };

    int result = 0;
    bool isOccupied = floor.contains(coord);
    
    for (const auto &offset : offsets) {
        Coord neighbour = {coord[0] + offset[0], coord[1] + offset[1]};
        if (floor.contains(neighbour))
            result++;
        if (isOccupied && !visited.contains(neighbour))
            toVisit.insert(neighbour);
    }

    return result;
}

Floor *nextFloor(const Floor *floor) {
    Floor *newFloor = new Floor();
    std::set<Coord> toVisit;
    std::set<Coord> visited;

    for (const auto &coord : *floor)
        toVisit.insert(coord);

    while (true) {
        const auto it = toVisit.cbegin();
        if (it == toVisit.cend())
            break;
        const Coord coord = *it;

        toVisit.erase(coord);
        visited.insert(coord);

        int adjacentFilled = calcAdjacentFilled(coord, *floor, visited, toVisit);
        bool isOccupied = floor->contains(coord);
        bool newOccupied;
        if (isOccupied)
            newOccupied = (adjacentFilled == 1) || (adjacentFilled == 2);
        else
            newOccupied = (adjacentFilled == 2);
        if (newOccupied)
            newFloor->insert(coord);
    }

    return newFloor;
}

Floor *initialFloor(const Input &input) {
    Floor *floor = new Floor();

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
        if (floor->contains(coord))
            floor->erase(coord);
        else
            floor->insert(coord);
    }

    return floor;
}

int answer(const Input &input) {
    std::shared_ptr<const Floor> floor(initialFloor(input));

    for (int day = 1; day <= 100; ++day) {
        std::shared_ptr<const Floor> newFloor(nextFloor(floor.get()));
        floor = newFloor;
    }

    return floor->size();
}

int main(void) {
    std::cout << answer(readParseInput("../input/day24.txt")) << '\n';
    return 0;
}
