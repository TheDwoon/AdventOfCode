#include "vec2.cpp"
#include "mat2.cpp"

namespace aoc::direction {
    static const vec2i UP { 0, -1 };
    static const vec2i DOWN { 0, 1 };
    static const vec2i LEFT { -1, 0};
    static const vec2i RIGHT { 1, 0 };
    static const vec2i DIRECTIONS[4] { UP, DOWN, LEFT, RIGHT };

    const static mat2i TURN_RIGHT {0, -1, 1, 0};
    const static mat2i TURN_LEFT {0, 1, -1, 0};

    static const vec2i NORTH { 0, -1 };
    static const vec2i SOUTH { 0, 1 };
    static const vec2i WEST { -1, 0};
    static const vec2i EAST { 1, 0 };

}