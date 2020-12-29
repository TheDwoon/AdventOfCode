#include "Day20.h"
#include <map>
#include <memory>
#include <deque>
#include <algorithm>
#include "vec2.h"
#include <assert.h>

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

        char flippedMap[TILE_SIZE * TILE_SIZE];

        // flip image if required
        if (flippedY) {
            for (int y = 0; y < TILE_SIZE; y++) {
                for (int x = 0; x < TILE_SIZE; x++) {
                    ACCESS_TILE(flippedMap, x, y) = ACCESS_TILE(tile.map, TILE_SIZE - 1 - x, y);                    
                }
            }
        }
        else if (flippedX) {
            for (int y = 0; y < TILE_SIZE; y++) {
                for (int x = 0; x < TILE_SIZE; x++) {
                    ACCESS_TILE(flippedMap, x, y) = ACCESS_TILE(tile.map, x, TILE_SIZE - 1 - y);
                }
            }
        }
        else {
            // I know, this problem could be better solved with lambdas modifying the access of tile.map; bla bla
            memcpy(flippedMap, tile.map, sizeof(flippedMap));
        }

        // copy map in corrected orientation
        for (int y = 0; y < TILE_SIZE; y++) {
            for (int x = 0; x < TILE_SIZE; x++) {
                ACCESS_TILE(map, x, y) = access_rotated(flippedMap, x, y, rotation);
            }
        }

        // read signatures because math is hard and I'm stupid :(
        topSignature = 0;
        bottomSignature = 0;
        for (int x = 0; x < TILE_SIZE; x++) {
            topSignature |= (ACCESS_TILE(map, x, 0) == FULL_SPOT) << (TILE_SIZE - 1 - x);
            bottomSignature |= (ACCESS_TILE(map, x, TILE_SIZE - 1) == FULL_SPOT) << (TILE_SIZE - 1 - x);
        }

        leftSignature = 0;
        rightSignature = 0;
        for (int y = 0; y < TILE_SIZE; y++) {
            leftSignature |= (ACCESS_TILE(map, 0, y) == FULL_SPOT) << (TILE_SIZE - 1 - y);
            rightSignature |= (ACCESS_TILE(map, TILE_SIZE - 1, y) == FULL_SPOT) << (TILE_SIZE - 1 - y);
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

    char getLocal(int x, int y) const {
        if (y < TILE_SIZE - 2 && y >= 0) {
            if (x < TILE_SIZE - 2 && x >= 0)
                return ACCESS_TILE(map, x + 1, y + 1);
            else
                if (right && x >= 0)
                    return right->getLocal(x - TILE_SIZE + 2, y);
                else
                    return EMPTY_SPOT;
        } else {
            if (bottom && y >= 0)
                return bottom->getLocal(x, y - TILE_SIZE + 2);
            else
                return EMPTY_SPOT;
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
    if (knownTiles.find(tile.number) != knownTiles.end()) {
        //std::cout << "Connected " << sourceTile << " <> " << tile.number << std::endl;
        return knownTiles[tile.number];
    }

    // find parameters to create new tile
    Facing requiredFacing = (expandDirection + 2) % 4;

    Facing signatureFacing;
    bool flippedX = false, flippedY = false;
    getSignatureFacing(tile, signature, signatureFacing, flippedX, flippedY);

    Facing rotation = (requiredFacing - signatureFacing + 4) % 4;

    //std::cout << "Connected " << sourceTile << " <> " << tile.number << std::endl;

    std::shared_ptr<PlacedTile> placedTile = std::make_shared<PlacedTile>(tile, rotation, flippedX, flippedY);
    knownTiles[placedTile->number] = placedTile;
    created = true;
    return placedTile;
}

int findSeaMonster(const std::vector<vec2i> &monster, const std::shared_ptr<PlacedTile> root) {
    int monsters = 0;

    for (int y = 0; y < 144; y++) {
        for (int x = 0; x < 144; x++) {
            bool found = true;
            for (int i = 0; found && i < monster.size(); i++) {
                const vec2i &v = monster[i];
                found = root->getLocal(v.x + x, v.y + y) == FULL_SPOT;
            }

            if (found)
                ++monsters;
        }
    }

    return monsters;
}

void printMap(const char* map) {
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            std::cout << map[y * 10 + x];
        }
        std::cout << std::endl;
    }
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
            assert(!placedTile->top || placedTile->top->bottomSignature == placedTile->topSignature);

            bool createdLeft = false;
            placedTile->left = connectTile(knownPlaces, input, placedTile->number, placedTile->leftSignature, WEST, createdLeft);
            assert(!placedTile->left || placedTile->left->rightSignature == placedTile->leftSignature);

            bool createdRight = false;
            placedTile->right = connectTile(knownPlaces, input, placedTile->number, placedTile->rightSignature, EAST, createdRight);
            assert(!placedTile->right || placedTile->right->leftSignature == placedTile->rightSignature);

            bool createdBottom = false;
            placedTile->bottom = connectTile(knownPlaces, input, placedTile->number, placedTile->bottomSignature, SOUTH, createdBottom);
            assert(!placedTile->bottom || placedTile->bottom->topSignature == placedTile->bottomSignature);

            /*std::cout << "> N:" << (placedTile->top ? placedTile->top->number : 0) << std::endl;
            if (placedTile->top) printMap(placedTile->top->map);
            
            std::cout << "### " << placedTile->number << std::endl;
            printMap(placedTile->map);
            
            std::cout << "> S:" << (placedTile->bottom ? placedTile->bottom->number : 0) << std::endl;
            if (placedTile->bottom) printMap(placedTile->bottom->map);
            
            std::cout << "> W:" << (placedTile->left ? placedTile->left->number : 0) << std::endl;
            if (placedTile->left) printMap(placedTile->left->map);
            
            std::cout << "> E:" << (placedTile->right ? placedTile->right->number : 0) << std::endl;
            if (placedTile->right) printMap(placedTile->right->map);
            
            std::cout << std::endl << std::endl << std::endl;*/

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

    for (int y = 0; y < 24; y++) {
        for (int x = 0; x < 24; x++) {
            std::cout << root->getLocal(x, y);
        }
        std::cout << std::endl;
    }

    std::vector<vec2i> monster{ {0, 0}, {1, 1}, {4, 1}, {5, 0}, {6, 0},
                                {7, 1}, {10, 1}, {11, 0}, {12, 0}, {13, 1},
                              {16, 1}, {17, 0}, {18, 0}, {18, -1}, {19, 0}};

    int monsters = 0;
    for (int i = 0; i < 4; i++) {
        monsters = findSeaMonster(monster, root);
        if (monsters > 0)
            break;

        std::vector<vec2i> xFlippedMonster(monster.size());
        std::transform(monster.begin(), monster.end(), xFlippedMonster.begin(), [](const vec2i& v) { return vec2i {-v.x, v.y}; });
        monsters = findSeaMonster(xFlippedMonster, root);
        if (monsters > 0)
            break;

        std::vector<vec2i> yFlippedMonster(monster.size());
        std::transform(monster.begin(), monster.end(), yFlippedMonster.begin(), [](const vec2i& v) { return vec2i {v.x, -v.y}; });
        monsters = findSeaMonster(yFlippedMonster, root);
        if (monsters > 0)
            break;

        // rotate monster
        std::vector<vec2i> rotated;
        rotated.reserve(monster.size());
        for (const vec2i &v : monster) {
            rotated.emplace_back(v.y, -v.x);
        }

        monster = rotated;
    }

    std::cout << "MONSTERS: " << monsters << std::endl;

//    // output id map (TOASTER)
    int roughWaters = 0;
    std::shared_ptr<PlacedTile> yBase = root;
    while (yBase) {
        std::shared_ptr<PlacedTile> xBase = yBase;
        while (xBase) {
            std::cout << xBase->number << " ";

            for (int y = 0; y < TILE_SIZE - 2; y++) {
                for (int x = 0; x < TILE_SIZE - 2; x++) {
                    roughWaters += xBase->getLocal(x, y) == FULL_SPOT;
                }
            }

            xBase = xBase->right;
        }
        std::cout << std::endl;

        yBase = yBase->bottom;
    }

    output << roughWaters - monsters * monster.size();

    return output.str();
}

bool Day20::testPart1(std::vector<Tile> &input) {
    std::string output = runPart1(input);
    return output == "20899048083289";
}

bool Day20::testPart2(std::vector<Tile> &input) {
    Tile& tile = input[3];
    printMap(tile.map);

    std::cout << std::endl;
    auto t0 = std::make_shared<PlacedTile>(tile, NORTH, false, false);
    printMap(t0->map);

    std::cout << std::endl;
    auto t1 = std::make_shared<PlacedTile>(tile, WEST, true, false);
    printMap(t1->map);

    std::string output = runPart2(input);
    return false;
    return output == "273";
}
