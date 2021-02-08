#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <map>
#include <set>
#include <string>
#include <vector>

using TileRow = std::string;
using Tile = std::vector<TileRow>;
using TileId = int;
using Input = std::map<TileId, Tile>;
using EdgeId = int;

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

template <typename T>
EdgeId calcEdgeId(T begin, T end) {
    T it(begin);
    EdgeId edgeId = 0;
    while (it != end) {
        edgeId = (edgeId << 1) | (*it == '#');
        it++;
    }
    return edgeId;
}

EdgeId topEdgeId(const Tile &tile) {
    const TileRow &row = tile[0];
    return calcEdgeId(row.cbegin(), row.cend());
}

EdgeId bottomEdgeId(const Tile &tile) {
    const TileRow &row = tile[tile.size() - 1];
    return calcEdgeId(row.cbegin(), row.cend());
}

struct TileEdgeIterator {
    TileEdgeIterator(const TileRow *row_, TileRow::size_type index_) :
        row(row_),
        index(index_)
        {}
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = Tile::difference_type;
    using value_type        = TileRow::value_type;
    using pointer           = value_type *;
    using reference         = value_type &;
    value_type operator*() const { return (*row)[index]; }
    TileEdgeIterator& operator++(int) { row++; return *this; }
    friend bool operator!=(const TileEdgeIterator& a, const TileEdgeIterator& b) { return a.row != b.row; };
private:
    const TileRow *row;
    TileRow::size_type index;
};

EdgeId leftEdgeId(const Tile &tile) {
    TileEdgeIterator s(&tile[0], 0);
    TileEdgeIterator e(&tile[tile.size()], 0);
    return calcEdgeId(s, e);
}

EdgeId rightEdgeId(const Tile &tile) {
    const TileRow &row0 = tile[0];
    TileRow::size_type index = row0.size() - 1;
    TileEdgeIterator s(&tile[0], index);
    TileEdgeIterator e(&tile[tile.size()], index);
    return calcEdgeId(s, e);
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

struct TileMutation {
    Tile tile;
    std::array<EdgeId, 4> edgeIds;
    static const int EDGE_ID_TOP = 0;
    static const int EDGE_ID_RIGHT = 1;
    static const int EDGE_ID_BOTTOM = 2;
    static const int EDGE_ID_LEFT = 3;
};
const int MUTATIONS = 8;
using TileMutations = std::array<TileMutation, MUTATIONS>;

auto allMutations(const Tile &tile, bool fillEdgeIds = true) {
    TileMutations mutations;
    mutations[0].tile = tile;
    mutations[1].tile = rotated(mutations[0].tile);
    mutations[2].tile = rotated(mutations[1].tile);
    mutations[3].tile = rotated(mutations[2].tile);
    for (int i = 0; i < 4; i++)
        mutations[4 + i].tile = hflipped(mutations[i].tile);
    if (fillEdgeIds) {
        for (int i = 0; i < MUTATIONS; i++) {
            mutations[i].edgeIds[TileMutation::EDGE_ID_TOP] = topEdgeId(mutations[i].tile);    
            mutations[i].edgeIds[TileMutation::EDGE_ID_RIGHT] = rightEdgeId(mutations[i].tile);    
            mutations[i].edgeIds[TileMutation::EDGE_ID_BOTTOM] = bottomEdgeId(mutations[i].tile);    
            mutations[i].edgeIds[TileMutation::EDGE_ID_LEFT] = leftEdgeId(mutations[i].tile);    
        }
    }
    return mutations;
}

int answer(const Input &input) {
    std::map<TileId, TileMutations> tileIdToMutations;
    for (const auto &[tileId, tile] : input)
        tileIdToMutations[tileId] = allMutations(tile);

    std::map<TileId, std::set<EdgeId>> tileIdToTopEdgeIds;
    for (const auto &[tileId, mutations] : tileIdToMutations) {
        for (const TileMutation &mutation : mutations)
            tileIdToTopEdgeIds[tileId].insert(mutation.edgeIds[TileMutation::EDGE_ID_TOP]);
    }

    std::map<EdgeId, std::set<TileId>> edgeIdToTileId;
    for (const auto &[tileId, edgeIds] : tileIdToTopEdgeIds) {
        for (const EdgeId &edgeId : edgeIds)
            edgeIdToTileId[edgeId].insert(tileId);
    }

    std::set<EdgeId> borderIds;
    for (const auto &[edgeId, tileIds] : edgeIdToTileId) {
        if (tileIds.size() == 1)
            borderIds.insert(edgeId);
    }

    std::set<TileId> cornerTileIds;
    for (const auto &[tileId, edgeIds] : tileIdToTopEdgeIds) {
        int edgeCount = 0;
        for (const auto &edgeId : edgeIds) {
            if (borderIds.find(edgeId) != borderIds.end())
                edgeCount++;
        }
        // 2 edges, each forward and reversed
        if (edgeCount == 2 * 2)
            cornerTileIds.insert(tileId);
    }

    std::set<TileId> unplacedTileIds;
    for (const auto &[tileId, tile] : input)
        unplacedTileIds.insert(tileId);

    int idBoardSize = int(sqrt(input.size()));
    bool firstRow = true, firstCol = true;
    using Placement = std::pair<TileId, int>;
    std::vector<std::vector<Placement>> idBoard;
    std::vector<Placement> *idBoardRow;
    EdgeId prevRightEdgeId = 0;
    std::vector<EdgeId> curTopConstraints; 
    std::vector<EdgeId> nextTopConstraints; 

    while (unplacedTileIds.size()) {
        if (firstCol)
            idBoardRow = &idBoard.emplace_back();
        std::set<EdgeId> topConstraint;
        if (firstRow)
            topConstraint = borderIds;
        else
            topConstraint = {curTopConstraints[idBoardRow->size()]};
        std::set<EdgeId> leftConstraint;
        if (firstCol)
            leftConstraint = borderIds;
        else
            leftConstraint = {prevRightEdgeId};
        Placement placement;
        bool found = false;
        for (const auto &tileId : unplacedTileIds) {
            const TileMutations& tileMutations = tileIdToMutations[tileId];
            for (int mutationId = 0; mutationId < MUTATIONS; ++mutationId) {
                const TileMutation& tileMutation = tileMutations[mutationId];
                EdgeId leftEdgeId = tileMutation.edgeIds[TileMutation::EDGE_ID_LEFT];
                if (leftConstraint.find(leftEdgeId) == leftConstraint.end())
                    continue;
                EdgeId topEdgeId = tileMutation.edgeIds[TileMutation::EDGE_ID_TOP];
                if (topConstraint.find(topEdgeId) == topConstraint.end())
                    continue;
                placement = {tileId, mutationId};                    
                found = true;
                break;
            }
            if (found)
                break;
        }
        if (!found)
            throw 0;
        idBoardRow->push_back(placement);
        const TileMutations& tileMutationsToPlace = tileIdToMutations[placement.first];
        const TileMutation& tileMutationToPlace = tileMutationsToPlace[placement.second];
        nextTopConstraints.push_back(tileMutationToPlace.edgeIds[TileMutation::EDGE_ID_BOTTOM]);
        prevRightEdgeId = tileMutationToPlace.edgeIds[TileMutation::EDGE_ID_RIGHT];
        unplacedTileIds.erase(placement.first);
        firstCol = false;
        if (idBoardRow->size() == idBoardSize) {
            firstRow = false;
            firstCol = true;
            curTopConstraints = nextTopConstraints;
            nextTopConstraints = {};
        }
    }

    int tileH = 0;
    int tileW = 0;
    for (const auto &[tileId, tile] : input) {
        tileH = tile.size();
        tileW = tile[0].size();
        break;
    }

    Tile combinedBoard;
    for (int idBoardRowId = 0; idBoardRowId < idBoard.size(); ++idBoardRowId) {
        const auto &idBoardRow = idBoard[idBoardRowId];
        for (int tileRowId = 1; tileRowId < tileH - 1; ++tileRowId) {
            std::string &boardRow = combinedBoard.emplace_back();
            for (const auto &[tileId, mutationId] : idBoardRow) {
                const TileMutations& tileMutations = tileIdToMutations[tileId];
                const TileMutation& tileMutation = tileMutations[mutationId];
                boardRow = boardRow + tileMutation.tile[tileRowId].substr(1, tileW - 2);
            }
        }
    }
    TileMutations combinedBoards = allMutations(combinedBoard, false);

    const std::vector<std::string> monster{
        "                  # ",
        "#    ##    ##    ###",
        " #  #  #  #  #  #   ",
    };
    int monsterH = monster.size();
    int monsterW = monster[0].size();

    for (const auto &mutation : combinedBoards) {
        const Tile &combinedBoard(mutation.tile);
        int boardH = combinedBoard.size();
        int boardW = combinedBoard[0].size();

        int monsterCount = 0;
        for (int boardBaseY = 0; boardBaseY <= boardH - monsterH; ++boardBaseY) {
            for (int boardBaseX = 0; boardBaseX <= boardW - monsterW; ++boardBaseX) {
                bool monsterHere = true;
                for (int monsterY = 0; monsterY < monsterH; ++monsterY) {
                    int boardY = boardBaseY + monsterY;
                    const std::string &boardRow = combinedBoard[boardY];
                    for (int monsterX = 0; monsterX < monsterW; ++monsterX) {
                        int boardX = boardBaseX + monsterX;
                        if (monster[monsterY][monsterX] != '#')
                            continue;
                        if (boardRow[boardX] != '#') {
                            monsterHere = false;
                            break;
                        }
                    }
                    if (!monsterHere)
                        break;
                }
                if (monsterHere)
                    monsterCount++;
            }
        }
        if (!monsterCount)
            continue;

        auto countHashes = [](const Tile &tile) {
            int hashes = 0;
            for (const auto &row : tile) {
                for (char c : row) {
                    if (c == '#')
                        hashes++;
                }
            }
            return hashes;
        };

        int roughness = countHashes(combinedBoard) - (monsterCount * countHashes(monster));
        return roughness;
    }

    return 0;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day20.txt")) << '\n';
    return 0;
}
