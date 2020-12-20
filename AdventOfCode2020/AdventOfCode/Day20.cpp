#include "Day20.h"
#include <map>

std::vector<Tile> Day20::parseInput(std::string &input) {
    std::vector<Tile> parsed;
    std::stringstream stream(input);

    char c;
    while (stream && stream.get(c).get(c).get(c).get(c).get(c)) {
        Tile tile;
        stream >> tile.number;
        stream.get(c);
        char* map = tile.map;
        for (int i = 0; i < TILE_SIZE * TILE_SIZE; i++)
            stream >> *(map++);

        // consume both \n
        stream.get(c).get(c);

        // compute signatures
        tile.topFlippedSignature = 0;
        tile.topSignature = 0;
        tile.bottomFlippedSignature = 0;
        tile.bottomSignature = 0;
        for (int x = 0; x < TILE_SIZE; x++) {
            tile.topSignature |= (ACCESS_TILE(tile.map, x, 0) == FULL_SPOT) << (TILE_SIZE - 1 - x);
            tile.topFlippedSignature |= (ACCESS_TILE(tile.map, x, 0) == FULL_SPOT) << (x);

            tile.bottomSignature |= (ACCESS_TILE(tile.map, x, TILE_SIZE - 1) == FULL_SPOT) << (TILE_SIZE - 1 - x);
            tile.bottomFlippedSignature |= (ACCESS_TILE(tile.map, x, TILE_SIZE - 1) == FULL_SPOT) << (x);
        }

        tile.leftFlippedSignature = 0;
        tile.leftSignature = 0;
        tile.rightFlippedSignature = 0;
        tile.rightSignature = 0;
        for (int y = 0; y < TILE_SIZE; y++) {
            tile.leftSignature |= (ACCESS_TILE(tile.map, 0, y) == FULL_SPOT) << (TILE_SIZE - 1 - y);
            tile.leftFlippedSignature |= (ACCESS_TILE(tile.map, 0, y) == FULL_SPOT) << (y);

            tile.rightSignature |= (ACCESS_TILE(tile.map, TILE_SIZE - 1, y) == FULL_SPOT) << (TILE_SIZE - 1 - y);
            tile.rightFlippedSignature |= (ACCESS_TILE(tile.map, TILE_SIZE - 1, y) == FULL_SPOT) << (y);
        }

        parsed.push_back(tile);
    }

    return parsed;
}

std::string Day20::runPart1(std::vector<Tile> &input) {
    std::stringstream output;

    long long result = 1;
    for (Tile &tile : input) {
        int matches = 0;
        for (Tile &otherTile : input) {
            if (otherTile.number == tile.number)
                continue;

            matches += otherTile.contains(tile.topSignature);
            matches += otherTile.contains(tile.leftSignature);
            matches += otherTile.contains(tile.rightSignature);
            matches += otherTile.contains(tile.bottomSignature);
        }

        if (matches == 2) {
            result *= tile.number;
        }
    }

    output << result;
    return output.str();
}
