#include "Day17.h"
#include <string.h>

#define IN_CUBE3(dim, x, y, z) ((x) >= 0 && (x) < (dim) && (y) >= 0 && (y) < (dim) && (z) >= 0 && (z) < (dim))
#define ACCESS_CUBE3(cube, dim, x, y, z) (cube[(z) * (dim) * (dim) + (y) * (dim) + (x)])

#define IN_CUBE4(dim, x, y, z, w) ((x) >= 0 && (x) < (dim) && (y) >= 0 && (y) < (dim) && (z) >= 0 && (z) < (dim) && (w) >= 0 && (w) < (dim))
#define ACCESS_CUBE4(cube, dim, x, y, z, w) (cube[(w) * (dim) * (dim) * (dim) + (z) * (dim) * (dim) + (y) * (dim) + (x)])

std::vector<char> Day17::parseInput(std::string &input) {
    std::vector<char> parsed;
    std::stringstream stream(input);

    char c;
    while (stream && stream >> c)
        if (c != '\n')
            parsed.push_back(c);

    return parsed;
}


size_t runSimulation3(size_t steps, unsigned char* &cube, unsigned char* &buffer, size_t cubeDimension) {
    // zero out buffer
    size_t aliveCells;
    while(steps--) {
        aliveCells = 0;
        memset(buffer, 0, cubeDimension * cubeDimension * cubeDimension);

        for (int z = 0; z < cubeDimension; z++) {
            for (int y = 0; y < cubeDimension; y++) {
                for (int x = 0; x < cubeDimension; x++) {
                    // For each position in the cube
                    size_t aliveNeighbors = 0;
                    for (int dz = -1; dz <= 1; dz++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            for (int dx = -1; dx <= 1; dx++) {
                                if (dz == 0 && dy == 0 && dx == 0)
                                    continue;

                                if (IN_CUBE3(cubeDimension, x + dx, y + dy, z + dz)) {
                                    aliveNeighbors += ACCESS_CUBE3(cube, cubeDimension, x + dx, y + dy, z + dz);
                                }
                            }
                        }
                    }

                    unsigned char cell = ACCESS_CUBE3(cube, cubeDimension, x, y, z);
                    unsigned char nextAlive = aliveNeighbors == 3 || cell == 1 && aliveNeighbors == 2;
                    ACCESS_CUBE3(buffer, cubeDimension, x, y, z) = nextAlive;
                    aliveCells += nextAlive;
                }
            }
        }

        std::swap(cube, buffer);
    }

    return aliveCells;
}

std::string Day17::runPart1(day_t& input) {
    std::stringstream output;

    const size_t cubeDimension = 25;
    unsigned char* buffer = new unsigned char[cubeDimension * cubeDimension * cubeDimension];
    unsigned char* cube = new unsigned char[cubeDimension * cubeDimension * cubeDimension];
    memset(cube, 0, cubeDimension * cubeDimension * cubeDimension);

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            ACCESS_CUBE3(cube, cubeDimension, x + 8, y + 8, 12) = input[y * 8 + x] == '#';
        }
    }

    output << runSimulation3(6, cube, buffer, cubeDimension);

    delete[] buffer;
    delete[] cube;
    return output.str();
}

size_t runSimulation4(size_t steps, unsigned char*& cube, unsigned char*& buffer, size_t cubeDimension) {
    // zero out buffer
    size_t aliveCells = 0;
    while (steps--) {
        aliveCells = 0;
        memset(buffer, 0, cubeDimension * cubeDimension * cubeDimension * cubeDimension);

        for (int w = 0; w < cubeDimension; w++) {
            for (int z = 0; z < cubeDimension; z++) {
                for (int y = 0; y < cubeDimension; y++) {
                    for (int x = 0; x < cubeDimension; x++) {
                        // For each position in the cube
                        size_t aliveNeighbors = 0;
                        for (int dw = -1; dw <= 1; dw++) {
                            for (int dz = -1; dz <= 1; dz++) {
                                for (int dy = -1; dy <= 1; dy++) {
                                    for (int dx = -1; dx <= 1; dx++) {
                                        if (dw == 0 && dz == 0 && dy == 0 && dx == 0)
                                            continue;

                                        if (IN_CUBE4(cubeDimension, x + dx, y + dy, z + dz, w + dw)) {
                                            aliveNeighbors += ACCESS_CUBE4(cube, cubeDimension, x + dx, y + dy, z + dz, w + dw);
                                        }
                                    }
                                }
                            }
                        }

                        unsigned char cell = ACCESS_CUBE4(cube, cubeDimension, x, y, z, w);
                        unsigned char nextAlive = aliveNeighbors == 3 || cell == 1 && aliveNeighbors == 2;
                        ACCESS_CUBE4(buffer, cubeDimension, x, y, z, w) = nextAlive;
                        aliveCells += nextAlive;
                    }
                }
            }
        }

        std::swap(cube, buffer);
    }

    return aliveCells;
}

std::string Day17::runPart2(day_t& input)
{
    std::stringstream output;

    const size_t cubeDimension = 25;
    unsigned char* buffer = new unsigned char[cubeDimension * cubeDimension * cubeDimension * cubeDimension];
    unsigned char* cube = new unsigned char[cubeDimension * cubeDimension * cubeDimension * cubeDimension];
    memset(cube, 0, cubeDimension * cubeDimension * cubeDimension * cubeDimension);

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            ACCESS_CUBE4(cube, cubeDimension, x + 8, y + 8, 12, 12) = input[y * 8 + x] == '#';
        }
    }

    output << runSimulation4(6, cube, buffer, cubeDimension);

    delete[] buffer;
    delete[] cube;
    return output.str();
}