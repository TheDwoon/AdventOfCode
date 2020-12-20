#include "Day20.h"
#include <map>
#include <memory>
#include <deque>
#include <algorithm>

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

// this is to make math :) Make math not war!
typedef char Facing;
typedef char Rotation;
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

void getSignatureFacing(const Tile& tile, unsigned short signature, Facing &facing, bool &flippedX, bool &flippedY) {
    if (signature == tile.topSignature || signature == tile.topFlippedSignature) {
        facing = NORTH;
        flippedY = signature == tile.topFlippedSignature;
    } else if (signature == tile.rightSignature || signature == tile.rightFlippedSignature) {
        facing = EAST;
        flippedX = signature == tile.rightFlippedSignature;
    } else if (signature == tile.bottomSignature || signature == tile.bottomFlippedSignature) {
        facing = SOUTH;
        flippedY = signature == tile.bottomFlippedSignature;
    } else if (signature == tile.leftSignature || signature == tile.leftFlippedSignature) {
        facing = WEST;
        flippedX = signature == tile.leftFlippedSignature;
    } else {
        throw std::exception();
    }
}

unsigned short getOrientedSignature(const Tile& tile, Facing facing, bool flipped) {
    switch (facing) {
        case NORTH:
            return flipped ? tile.topFlippedSignature : tile.topSignature;
        case SOUTH:
            return flipped ? tile.bottomFlippedSignature : tile.bottomSignature;
        case WEST:
            return flipped ? tile.leftFlippedSignature : tile.leftSignature;
        case EAST:
            return flipped? tile.rightFlippedSignature : tile.rightSignature;
        default:
            throw std::exception();   // should never happen (I hope)
    }
}

char access_rotated(const char *map, int x, int y, Facing rotation) {
    if (rotation < 0)
        rotation += 4;

    switch (rotation) {
        case 0:
            return ACCESS_TILE(map, x, y);
        case 1:
            // rotate 90 degrees
            return ACCESS_TILE(map, y, TILE_SIZE - 1 - x);
        case 2:
            // rotate 180 degrees
            return ACCESS_TILE(map, TILE_SIZE - 1 - x, TILE_SIZE - 1 - y);
        case 3:
            // rotate 270 degrees
            return ACCESS_TILE(map, TILE_SIZE - 1 - y, x);
        default:
            throw std::exception();
    }
}

struct PlacedTile {
    std::shared_ptr<PlacedTile> top;
    std::shared_ptr<PlacedTile> left;
    std::shared_ptr<PlacedTile> right;
    std::shared_ptr<PlacedTile> bottom;

    unsigned short topSignature;
    unsigned short rightSignature;
    unsigned short bottomSignature;
    unsigned short leftSignature;

    int number;
    char map[TILE_SIZE * TILE_SIZE];

    PlacedTile(const Tile& tile, Rotation rotation, bool flippedX, bool flippedY) {
        number = tile.number;
        std::cout << "PlacedTile: " << number << " r:" << (int)rotation << " flipped: " << flippedX << " " << flippedY << std::endl;

        // FIXME: this may be wrong
        topSignature = getOrientedSignature(tile, rotation, flippedY);
        rightSignature = getOrientedSignature(tile, (rotation + 1) % 4, flippedX);
        bottomSignature = getOrientedSignature(tile, (rotation + 2) % 4, flippedY);
        leftSignature = getOrientedSignature(tile, (rotation + 3) % 4, flippedX);

        if (flippedX)
            std::swap(topSignature, bottomSignature);
        if (flippedY)
            std::swap(leftSignature, rightSignature);

        // copy map in corrected orientation
        for (int y = 0; y < TILE_SIZE; y++) {
            for (int x = 0; x < TILE_SIZE; x++) {
                ACCESS_TILE(map, x, y) = access_rotated(tile.map, x, y, rotation);
            }
        }

        // flip image if required
        if (flippedY) {
            for (int y = 0; y < TILE_SIZE; y++) {
                for (int x = 0; x < TILE_SIZE / 2; x++) {
                    std::swap(ACCESS_TILE(map, x, y), ACCESS_TILE(map, TILE_SIZE - 1 - x, y));
                }
            }
        } else if (flippedX) {
            for (int y = 0; y < TILE_SIZE / 2; y++) {
                for (int x = 0; x < TILE_SIZE; x++) {
                    std::swap(ACCESS_TILE(map, x, y), ACCESS_TILE(map, x, TILE_SIZE - 1 - y));
                }
            }
        }
    }

    char getGlobal(int x, int y) const {
        if (y < TILE_SIZE) {
            if (x < TILE_SIZE)
                return ACCESS_TILE(map, x, y);
            else
                return right->getGlobal(x - TILE_SIZE, y);
        } else {
            return bottom->getGlobal(x, y - TILE_SIZE);
        }
    }
};

std::shared_ptr<PlacedTile> connectTile(std::map<int, std::shared_ptr<PlacedTile>> &knownTiles, const std::vector<Tile> &tiles,
                                        int sourceTile, unsigned short signature, Facing expandDirection, bool &created) {
    // search for tile matching given signature
    auto match = std::find_if(tiles.begin(), tiles.end(), [signature, sourceTile] (const Tile& t) { return t.number != sourceTile && t.contains(signature); });
    if (match == tiles.end())
        return std::shared_ptr<PlacedTile>();

    // check if tile is already placed in map
    const Tile& tile = *match;
    if (knownTiles.find(tile.number) != knownTiles.end())
        return knownTiles[tile.number];

    // find parameters to create new tile
    Facing requiredFacing = (expandDirection + 2) % 4;

    Facing signatureFacing;
    bool flippedX = false, flippedY = false;
    getSignatureFacing(tile, signature, signatureFacing, flippedX, flippedY);

    Facing rotation = (requiredFacing - signatureFacing + 4) % 4;

    std::shared_ptr<PlacedTile> placedTile = std::make_shared<PlacedTile>(tile, rotation, flippedX, flippedY);
    knownTiles[placedTile->number] = placedTile;
    created = true;
    return placedTile;
}

void printNeighborIds(std::shared_ptr<PlacedTile> tile) {
    std::cout << "Tile: " << tile->number;
    if (tile->top)
        std::cout << " Top: " << tile->top->number;
}

std::string Day20::runPart2(std::vector<Tile> &input) {
    std::stringstream output;

    std::shared_ptr<PlacedTile> root = std::make_shared<PlacedTile>(input[0], NORTH, false, false);
    {
        std::map<int, std::shared_ptr<PlacedTile>> knownPlaces;
        knownPlaces[root->number] = root;

        std::deque<std::shared_ptr<PlacedTile>> queue { root };
        while (!queue.empty()) {
            std::shared_ptr<PlacedTile> placedTile = queue.front();
            queue.pop_front();

            bool createdTop = false;
            placedTile->top = connectTile(knownPlaces, input, placedTile->number, placedTile->topSignature, NORTH, createdTop);
            bool createdLeft = false;
            placedTile->left = connectTile(knownPlaces, input, placedTile->number, placedTile->leftSignature, WEST, createdLeft);
            bool createdRight = false;
            placedTile->right = connectTile(knownPlaces, input, placedTile->number, placedTile->rightSignature, EAST, createdRight);
            bool createdBottom = false;
            placedTile->bottom = connectTile(knownPlaces, input, placedTile->number, placedTile->bottomSignature, SOUTH, createdBottom);

            if (createdTop) queue.push_back(placedTile->top);
            if (createdRight) queue.push_back(placedTile->right);
            if (createdLeft) queue.push_back(placedTile->left);
            if (createdBottom) queue.push_back(placedTile->bottom);
        }
    }

    // move base to top left
    while (root->top) {
        root = root->top;
    }
    while (root->left)
        root = root->left;

    for (int y = 0; y < 30; y++) {
        for (int x = 0; x < 30; x++) {
            std::cout << root->getGlobal(x, y);
            if ((x + 1) % 10 == 0)
                std::cout << " ";
        }
        std::cout << std::endl;
        if ((y + 1) % 10 == 0)
            std::cout << std::endl;
    }

//    // output id map
//    std::shared_ptr<PlacedTile> yBase = root;
//    while (yBase) {
//        std::shared_ptr<PlacedTile> xBase = yBase;
//        while (xBase) {
//            std::cout << xBase->number << " ";
//            xBase = xBase->right;
//        }
//        std::cout << std::endl;
//
//        yBase = yBase->bottom;
//    }

    output << root->number;

    return output.str();
}
