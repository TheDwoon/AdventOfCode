#include "Day12.h"
#include "vec2.h"

const vec2d DIRECTION_NORTH(0, 1);
const vec2d DIRECTION_SOUTH(0, -1);
const vec2d DIRECTION_WEST(-1, 0);
const vec2d DIRECTION_EAST(1, 0);

const double DEG_TO_RAD = M_PI / 180.0;

std::vector<MovementInstruction> Day12::parseInput(std::string &input) {
    std::vector<MovementInstruction> parsed;
    std::stringstream stream(input);

    char c;
    int i;
    while (stream && stream >> c >> i)
        parsed.push_back(MovementInstruction{c, i});

    return parsed;
}

std::string Day12::runPart1(std::vector<MovementInstruction> &input) {
    std::stringstream output;

    vec2d heading = DIRECTION_EAST;
    vec2d ship {0, 0};
    for (MovementInstruction& instruction : input) {
        switch (instruction.operation) {
            case NORTH:
                ship += DIRECTION_NORTH * instruction.distance;
                break;
            case SOUTH:
                ship += DIRECTION_SOUTH * instruction.distance;
                break;
            case EAST:
                ship += DIRECTION_EAST * instruction.distance;
                break;
            case WEST:
                ship += DIRECTION_WEST * instruction.distance;
                break;
            case LEFT:
                heading = heading.rotate(instruction.distance * DEG_TO_RAD);
                break;
            case RIGHT:
                heading = heading.rotate(-instruction.distance * DEG_TO_RAD);
                break;
            case FORWARD:
                ship += heading * instruction.distance;
                break;
            default:
                std::cout << "Unknown Instruction! " << instruction.operation << std::endl;
                break;
        }
    }

    output << (abs(ship.x) + abs(ship.y));
    return output.str();
}

std::string Day12::runPart2(std::vector<MovementInstruction> &input) {
    std::stringstream output;

    vec2d ship {0, 0};
    vec2d waypoint {10, 1};

    for (MovementInstruction& instruction : input) {
        switch (instruction.operation) {
            case NORTH:
                waypoint += (DIRECTION_NORTH * instruction.distance);
                break;
            case SOUTH:
                waypoint += (DIRECTION_SOUTH * instruction.distance);
                break;
            case EAST:
                waypoint += (DIRECTION_EAST * instruction.distance);
                break;
            case WEST:
                waypoint += (DIRECTION_WEST * instruction.distance);
                break;
            case LEFT:
                waypoint = waypoint.rotate(instruction.distance * DEG_TO_RAD);
                break;
            case RIGHT:
                waypoint = waypoint.rotate(-instruction.distance * DEG_TO_RAD);
                break;
            case FORWARD: {
                ship += waypoint * instruction.distance;
                break;
            }
            default:
                std::cout << "Unknown operation: " << instruction.operation << std::endl;
                break;
        }
    }

    output << (abs(ship.x) + abs(ship.y));
    return output.str();
}
