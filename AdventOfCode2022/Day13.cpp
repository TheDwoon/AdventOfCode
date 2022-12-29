#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <cassert>

#define TITLE "Day 13"
#define TYPE_LIST 1
#define TYPE_LITERAL 2

#define WRONG_ORDER -1
#define UNDECIDED_ORDER 0
#define CORRECT_ORDER 1

struct packet {
    packet(uint8_t type) : type(type) {}

    uint8_t type;
};

struct list_packet : packet {
    list_packet() : packet(TYPE_LIST) {}

    std::vector<const packet*> packets;
};

struct literal_packet : packet {
    literal_packet() : packet(TYPE_LITERAL) {}

    int value { 0 };
};

std::string toString(const packet* p) {
    if (p->type == TYPE_LITERAL) {
        const auto* lp = reinterpret_cast<const literal_packet*>(p);
        return std::to_string(lp->value);
    } else if (p->type == TYPE_LIST) {
        const auto* lp = reinterpret_cast<const list_packet*>(p);
        std::stringstream stream;
        stream << "[";
        for (size_t i = 0; i < lp->packets.size(); ++i) {
            if (i != 0)
                stream << ", ";

            stream << toString(lp->packets[i]);
        }
        stream << "]";
        return stream.str();
    }

    return "<null>";
}

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<list_packet*> day_t;

list_packet* parsePacket(std::stringstream &stream) {
    list_packet* packet = new list_packet();
    char c;
    stream.get(c);
    assert(c == '[');

    while (stream.peek() != ']') {
        if (stream.peek() == ',')
            stream.get();

        int peek = stream.peek();
        if (peek == '[') {
            packet->packets.push_back(parsePacket(stream));
        } else if (peek != ']') {
            auto* literal = new literal_packet();
            stream >> literal->value;
            assert(stream.good());
            packet->packets.push_back(literal);
            if (stream.peek() == ',')
                stream.get();
        }
    }

    assert(stream.peek() == ']');
    stream.get();

    return packet;
}

day_t parseInput(std::string &input) {
    day_t parsed;

    std::string line;
    std::stringstream stream(input);
    while (std::getline(stream, line)) {
        if (line.empty())
            continue;

        std::stringstream lineStream(line);
        parsed.push_back(parsePacket(lineStream));
    }

    return parsed;
}

int isOrdered(const list_packet* a, const list_packet* b) {
#ifndef NDEBUG
    std::cout << toString(a) << " vs " << toString(b) << std::endl;
#endif

    const size_t limit = std::min(a->packets.size(), b->packets.size());
    for (size_t i = 0; i < limit; ++i) {
        const packet* subA = a->packets[i];
        const packet* subB = b->packets[i];

        if (subA->type == TYPE_LITERAL && subB->type == TYPE_LITERAL) {
            auto* literalA = reinterpret_cast<const literal_packet*>(subA);
            auto* literalB = reinterpret_cast<const literal_packet*>(subB);

#ifndef NDEBUG
            std::cout << toString(subA) << " vs " << toString(subB) << std::endl;
#endif

            if (literalA->value < literalB->value) {
                return CORRECT_ORDER;
            } else if (literalA->value > literalB->value) {
#ifndef NDEBUG
                std::cout << "left side is bigger" << std::endl;
#endif
                return WRONG_ORDER;
            }
        } else if (subA->type == TYPE_LIST && subB->type == TYPE_LIST) {
            auto* listA = reinterpret_cast<const list_packet*>(subA);
            auto* listB = reinterpret_cast<const list_packet*>(subB);

            if (isOrdered(listA, listB) == WRONG_ORDER) {
#ifndef NDEBUG
                std::cout << "left side is bigger" << std::endl;
#endif
                return WRONG_ORDER;
            }
        } else if (subA->type == TYPE_LITERAL) {
            auto* listA = new list_packet();
            listA->packets.push_back(subA);
            auto* listB = reinterpret_cast<const list_packet*>(subB);

            if (isOrdered(listA, listB) == WRONG_ORDER) {
#ifndef NDEBUG
                std::cout << "left side is bigger" << std::endl;
#endif
                return WRONG_ORDER;
            }

            delete listA;
        } else if (subB->type == TYPE_LITERAL) {
            auto* listA = reinterpret_cast<const list_packet*>(subA);
            auto* listB = new list_packet();
            listB->packets.push_back(subB);

            if (isOrdered(listA, listB) == WRONG_ORDER) {
#ifndef NDEBUG
                std::cout << "left side is bigger" << std::endl;
#endif
                return WRONG_ORDER;
            }

            delete listB;
        }
    }

    if (a->packets.size() == b->packets.size())
        return UNDECIDED_ORDER;
    else if (a->packets.size() < b->packets.size())
        return CORRECT_ORDER;
    else
        return WRONG_ORDER;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    int sum { 0 };
    for (int i = 0; i + 1 < input.size(); i += 2) {
        list_packet* first = input[i];
        list_packet* second = input[i + 1];

        if (isOrdered(first, second) != WRONG_ORDER) {
#ifndef NDEBUG
            std::cout << "### Pair " << (i + 2) / 2 << " in correct order!\n";
#endif
            sum += (i + 2) / 2;
        } else {
#ifndef NDEBUG
            std::cout << "### Pair " << (i + 2) / 2 << " NOT in correct order!\n";
#endif
        }
    }

    output << sum;
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
        output << t.count() / 1000 << "us";
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

	const std::string originalInput = readInput();

    std::string input = originalInput;
	auto t0 = std::chrono::high_resolution_clock::now();
	day_t parsedInput = parseInput(input);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::string output = runPart1(parsedInput);
	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout << std::endl;
	std::cout << "**************************************" << std::endl;
	std::cout << output << std::endl;
	std::cout << "*************** Task 1 ***************" << std::endl;
	std::cout << "Parsing: " << formatTime(t1 - t0) << std::endl;
	std::cout << "Running: " << formatTime(t2 - t1) << std::endl;
	std::cout << "Total: " << formatTime(t2 - t0) << std::endl;
	std::cout << "**************************************" << std::endl;

	input = originalInput;
	t0 = std::chrono::high_resolution_clock::now();
	parsedInput = parseInput(input);
	t1 = std::chrono::high_resolution_clock::now();
	output = runPart2(parsedInput);
	t2 = std::chrono::high_resolution_clock::now();

	std::cout << std::endl;
	std::cout << "**************************************" << std::endl;
	std::cout << output << std::endl;
	std::cout << "*************** Task 2 ***************" << std::endl;
	std::cout << "Parsing: " << formatTime(t1 - t0) << std::endl;
	std::cout << "Running: " << formatTime(t2 - t1) << std::endl;
	std::cout << "Total: " << formatTime(t2 - t0) << std::endl;
	std::cout << "**************************************" << std::endl;
}

std::vector<std::string> tokenize(const std::string &input, const std::string &separator) {
    std::vector<std::string> tokenized;

    size_t pos = 0;
    size_t tokenEnd;
    do {
        tokenEnd = input.find(separator, pos);
        std::string token = input.substr(pos, (tokenEnd - pos));
        tokenized.push_back(token);
        pos = tokenEnd + separator.size();
    } while(tokenEnd != std::string::npos);

    return tokenized;
}
