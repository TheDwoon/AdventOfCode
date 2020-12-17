#include "Day17.h"
#include <string.h>

#define IN_CUBE(dim, x, y, z) ((x) >= 0 && (x) < (dim) && (y) >= 0 && (y) < (dim) && (z) >= 0 && (z) < (dim))
#define ACCESS_CUBE(cube, dim, x, y, z) (cube[(z) * (dim) * (dim) + (y) * (dim) + (x)])

std::vector<char> Day17::parseInput(std::string &input) {
    std::vector<char> parsed;
    std::stringstream stream(input);

    char c;
    while (stream && stream >> c)
        if (c != '\n')
            parsed.push_back(c);

    return parsed;
}

int runSimulation(size_t steps, unsigned char* &cube, unsigned char* &buffer, size_t cubeDimension) {
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

                                if (IN_CUBE(cubeDimension, x + dx, y + dy, z + dz)) {
                                    aliveNeighbors += ACCESS_CUBE(cube, cubeDimension, x + dx, y + dy, z + dz);
                                }
                            }
                        }
                    }

                    unsigned char cell = ACCESS_CUBE(cube, cubeDimension, x, y, z);
                    unsigned char nextAlive =  aliveNeighbors == 3 || cell == 1 && aliveNeighbors == 2;
                    ACCESS_CUBE(buffer, cubeDimension, x, y, z) = nextAlive;
                    aliveCells += nextAlive;
                }
            }
        }

        std::swap(cube, buffer);
    }

    return aliveCells;
}

std::string Day17::runPart1(std::vector<char> &input) {
    std::stringstream output;

    const size_t cubeDimension = 25;
    unsigned char* buffer = new unsigned char[cubeDimension * cubeDimension * cubeDimension];
    unsigned char* cube = new unsigned char[cubeDimension * cubeDimension * cubeDimension];
    memset(cube, 0, cubeDimension * cubeDimension * cubeDimension);

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            ACCESS_CUBE(cube, cubeDimension, x + 8, y + 8, 12) = input[y * 8 + x];
        }
    }

    output << runSimulation(6, cube, buffer, cubeDimension);

    delete[] buffer;
    delete[] cube;
    return output.str();
}
