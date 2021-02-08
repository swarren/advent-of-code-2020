#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using TileRow = std::string;
using Tile = std::vector<TileRow>;
using TileId = int;
using Input = std::map<TileId, Tile>;

std::ostream &operator<<(std::ostream &os, const Tile &tile) {
    for (const auto &line : tile)
        std::cout << line << '\n';
    return os;
}

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    Tile *tile = nullptr;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.length())
            continue;
        if (line.starts_with("Tile ")) {
            TileId tileId = atoi(&line[5]);
            tile = &(input.emplace(tileId, Tile{}).first->second);
        } else {
            tile->push_back(line);
        }
    }
    file.close();

    return input;
}

Tile rotated(const Tile& tile) {
    using ST = std::string::size_type;
    ST curH = tile.size();
    ST curW = tile[0].size();
    ST newH = curW;
    ST newW = curH;
    Tile newTile(newH);
    for (TileRow &row : newTile)
        row.resize(newW);
    for (ST y = 0; y < newH; ++y) {
        for (ST x = 0; x < newW; ++x) {
            newTile[y][x] = tile[curW - x - 1][y];
        }
    }    
    return newTile;
}

Tile hflipped(const Tile& tile) {
    Tile newTile;
    newTile.reserve(tile.size());
    for (const std::string &line : tile) {
        std::string newLine(line);
        std::reverse(newLine.begin(), newLine.end());
        newTile.push_back(newLine);
    }
    return newTile;
}

auto allMutations(const Tile &tile) {
    std::array<Tile, 8> mutations;
    mutations[0] = tile;
    mutations[1] = rotated(mutations[0]);
    mutations[2] = rotated(mutations[1]);
    mutations[3] = rotated(mutations[2]);
    for (int i = 0; i < 4; i++)
        mutations[4 + i] = hflipped(mutations[i]);
    return mutations;
}

using EdgeId = int;

EdgeId topEdgeId(const Tile &tile) {
    const TileRow &row = tile[0];
    EdgeId edgeId = 0;
    using ST = std::string::size_type;
    for (ST i = 0; i < row.size(); ++i)
        edgeId = (edgeId << 1) | (row[i] == '#');
    return edgeId;
}

long answer(const Input &input) {
    std::map<TileId, std::set<EdgeId>> tileToEdgeIds;
    for (const auto &[tileId, tile] : input) {
        for (const Tile &mutatedTile : allMutations(tile)) {
            EdgeId edgeId = topEdgeId(mutatedTile);
            tileToEdgeIds[tileId].insert(edgeId);
        }
    }

    std::map<EdgeId, std::set<TileId>> edgeIdToTiles;
    for (const auto &[tileId, edgeIds] : tileToEdgeIds) {
        for (const EdgeId &edgeId : edgeIds)
            edgeIdToTiles[edgeId].insert(tileId);
    }

    std::set<EdgeId> borderIds;
    for (const auto &[edgeId, tileIds] : edgeIdToTiles) {
        if (tileIds.size() == 1)
            borderIds.insert(edgeId);
    }

    std::set<TileId> cornerTileIds;
    for (const auto &[tileId, edgeIds] : tileToEdgeIds) {
        int count = 0;
        for (const auto &edgeId : edgeIds) {
            if (borderIds.find(edgeId) != borderIds.end())
                count++;
        }
        if (count == 4)
            cornerTileIds.insert(tileId);
    }

    long answer = 1;
    for (const auto &tileId : cornerTileIds)
        answer *= tileId;

    return answer;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day20.txt")) << '\n';
    return 0;
}
