#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <deque>
#include "parser.cpp"
#include "vec2.cpp"

#define TITLE "Day 16"

#define EMPTY_SPACE '.'
#define MIRROR_1 '/'
#define MIRROR_2 '\\'
#define SPLITTER_HORIZONTAL '-'
#define SPLITTER_VERTICAL '|'

#define TYPE_MASK 0b111
#define C_EMPTY_SPACE 0
#define C_MIRROR_1 1
#define C_MIRROR_2 2
#define C_SPLITTER_HORIZONTAL 3
#define C_SPLITTER_VERTICAL 4

#define BEAM_MASK 0b1111000
#define BEAM_NORTH 0b1000000
#define BEAM_EAST 0b0100000
#define BEAM_SOUTH 0b0010000
#define BEAM_WEST 0b0001000

struct map {
    std::vector<unsigned char> data;
    int width {0};
    int height {0};

    unsigned char& operator()(const vec2i &pos) {
        assert(pos.x >= 0 && pos.x < width);
        assert(pos.y >= 0 && pos.y < height);
        return data[pos.y * width + pos.x];
    }

    unsigned char& operator()(int x, int y) {
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
        return data[y * width + x];
    }
};

struct ray {
    vec2i pos;
    unsigned char direction;
};

typedef map day_t;

day_t parseInput(std::string &input) {
    day_t result;
    Parser p(input.c_str());
    while (!p.eof()) {
        while (!p.isNewLine()) {
            char c = p.read();
            unsigned char code;
            switch (c) {
                case EMPTY_SPACE: code = C_EMPTY_SPACE; break;
                case MIRROR_1: code = C_MIRROR_1; break;
                case MIRROR_2: code = C_MIRROR_2; break;
                case SPLITTER_HORIZONTAL: code = C_SPLITTER_HORIZONTAL; break;
                case SPLITTER_VERTICAL: code = C_SPLITTER_VERTICAL; break;
                default: throw std::exception();
            }

            result.data.push_back(code);
        }

        p.readNewLine();

        if (result.width == 0)
            result.width = static_cast<int>(result.data.size());
    }

    assert(static_cast<int>(result.data.size()) % result.width == 0);
    result.height = static_cast<int>(result.data.size()) / result.width;

    return result;
}

bool isEmptySpace(unsigned char code) {
    return (code & TYPE_MASK) == 0;
}

bool isMirror(unsigned char code) {
    code = code & TYPE_MASK;
    return code == C_MIRROR_1 || code == C_MIRROR_2;
}

bool isSplitter(unsigned char code, unsigned char direction) {
    code = code & TYPE_MASK;
    return (code == C_SPLITTER_HORIZONTAL && (direction == BEAM_NORTH || direction == BEAM_SOUTH))
        || (code == C_SPLITTER_VERTICAL && (direction == BEAM_WEST || direction == BEAM_EAST));
}

bool canWalk(unsigned char code, unsigned char direction) {
    return !isSplitter(code, direction) && !isMirror(code);
}

bool hasBeam(unsigned char code, unsigned char direction) {
    assert(direction != 0);
    return (code & direction) == direction;
}

bool hasBeam(unsigned char code) {
    return (code & BEAM_MASK) != 0;
}

bool inMap(const map& m, const vec2i &pos) {
    return pos.x >= 0 && pos.x < m.width && pos.y >= 0 && pos.y < m.height;
}

unsigned char getMirroredDirection(unsigned char mirrorCode, unsigned char direction) {
    bool mirror1 = (mirrorCode & TYPE_MASK) == C_MIRROR_1;
    switch (direction) {
        case BEAM_NORTH: return mirror1 ? BEAM_EAST : BEAM_WEST;
        case BEAM_SOUTH: return mirror1 ? BEAM_WEST : BEAM_EAST;
        case BEAM_WEST: return mirror1 ? BEAM_SOUTH : BEAM_NORTH;
        case BEAM_EAST: return mirror1 ? BEAM_NORTH : BEAM_SOUTH;
        default: throw std::exception();
    }
}

static const vec2i NORTH(0, -1);
static const vec2i EAST(1, 0);
static const vec2i SOUTH(0, 1);
static const vec2i WEST(-1, 0);

vec2i advance(const vec2i &pos, unsigned char direction) {
    switch (direction) {
        case BEAM_NORTH:
            return pos + NORTH;
        case BEAM_EAST:
            return pos + EAST;
        case BEAM_SOUTH:
            return pos + SOUTH;
        case BEAM_WEST:
            return pos + WEST;
        default: throw std::exception();
    }
}

void castRay(map &m, vec2i initialPosition, unsigned char initialDirection) {
    std::deque<ray> queue;
    queue.emplace_back(initialPosition, initialDirection);

    while (!queue.empty()) {
        vec2i pos = queue.front().pos;
        unsigned char direction = queue.front().direction;
        queue.pop_front();

        if (inMap(m, pos)) {
            unsigned char &c = m(pos);
            if (hasBeam(c, direction) && !isMirror(c))
                continue;
            else
                c |= direction;
        }

        pos = advance(pos, direction);
        bool followRay = true;
        while (followRay && inMap(m, pos)) {
            unsigned char &n = m(pos);
            n |= direction;

            if (isSplitter(n, direction)) {
                ray r1 { pos, 0 };
                ray r2 { pos, 0 };

                switch (direction) {
                    case BEAM_NORTH:
                    case BEAM_SOUTH:
                        r1.direction = BEAM_WEST;
                        r2.direction = BEAM_EAST;
                        break;
                    case BEAM_WEST:
                    case BEAM_EAST:
                        r1.direction = BEAM_NORTH;
                        r2.direction = BEAM_SOUTH;
                        break;
                    default: throw std::exception();
                }

                queue.push_back(r1);
                queue.push_back(r2);
                followRay = false;
            } else if (isMirror(n)) {
                queue.emplace_back(pos, getMirroredDirection(n, direction));
                followRay = false;
            }

            pos = advance(pos, direction);
        }
    }
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    castRay(input, {-1,0}, BEAM_EAST);

    int score = 0;
    for (int y = 0; y < input.height; y++) {
        for (int x = 0; x < input.width; x++) {
            if (hasBeam(input(x, y))) {
                score += 1;
                std::cout << '#';
            } else {
                std::cout << '.';
            }
        }
        std::cout << '\n';
    }

    output << score;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    return output.str();
}

// BOILER PLATE CODE BELOW

std::string readInput() {
    std::cin >> std::noskipws;

    std::istream_iterator<char> it(std::cin);
    std::istream_iterator<char> end;
    std::string fileContent(it, end);

    return fileContent;
}

std::string formatTime(std::chrono::duration<long long, std::nano> t) {
    std::stringstream output;
    if (t.count() < 10000) {
        output << t.count() << "ns";
    }
    else if (t.count() < 10000000) {
        output << t.count() / 1000 << "µs";
    }
    else {
        output << t.count() / 1000000 << "ms";
    }

    return output.str();
}

int main()
{
	std::cout << "######################################" << std::endl;
	std::cout << "############### " << TITLE << " ###############" << std::endl;
	std::cout << "######################################" << std::endl;

    auto gt0 = std::chrono::high_resolution_clock::now();
	const std::string originalInput = readInput();
    auto gt1 = std::chrono::high_resolution_clock::now();

    std::cout << "Reading Input: " << formatTime(gt1 - gt0) << '\n';
	std::cout << "**************************************\n";

    std::string input = originalInput;
	auto t0 = std::chrono::high_resolution_clock::now();
	day_t parsedInput = parseInput(input);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::string output = runPart1(parsedInput);
	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout << '\n';
	std::cout << "**************************************\n";
	std::cout << output << '\n';
	std::cout << "*************** Task 1 ***************\n";
	std::cout << "Parsing: " << formatTime(t1 - t0) << '\n';
	std::cout << "Running: " << formatTime(t2 - t1) << '\n';
	std::cout << "Total: " << formatTime(t2 - t0) << '\n';
	std::cout << "**************************************" << std::endl;

	input = originalInput;
	t0 = std::chrono::high_resolution_clock::now();
	parsedInput = parseInput(input);
	t1 = std::chrono::high_resolution_clock::now();
	output = runPart2(parsedInput);
	t2 = std::chrono::high_resolution_clock::now();

	std::cout << std::endl;
	std::cout << "**************************************\n";
	std::cout << output << '\n';
	std::cout << "*************** Task 2 ***************\n";
	std::cout << "Parsing: " << formatTime(t1 - t0) << '\n';
	std::cout << "Running: " << formatTime(t2 - t1) << '\n';
	std::cout << "Total: " << formatTime(t2 - t0) << '\n';
	std::cout << "**************************************" << std::endl;

    auto gt2 = std::chrono::high_resolution_clock::now();
    std::cout << "Global Time: " << formatTime(gt2 - gt0) << '\n';
	std::cout << "**************************************" << std::endl;
}
