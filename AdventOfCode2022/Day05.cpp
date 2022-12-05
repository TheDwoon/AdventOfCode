#include <string>
#include <iostream>
#include <chrono>
#include <iterator>
#include <vector>
#include <regex>
#include <list>

#define TITLE "Day 05"

typedef std::list<char> crate;

struct move {
    int amount;
    int source;
    int target;
};

struct puzzle {
    std::vector<crate> container;
    std::vector<move> moves;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef puzzle day_t;

day_t parseInput(const std::string &input) {
    day_t puzzle;

    std::stringstream stream(input);
    std::string line;
    while (std::getline(stream, line, '\n')) {
        if (line.find('[') == std::string::npos)
            break;

        if (puzzle.container.empty()) {
            int pain = (line.length() + 1) / 4;
            for (int i = 0; i < pain; i++) {
                crate c;
                puzzle.container.emplace_back(c);
            }
        }

        for (int i = 1; i < line.length(); i += 4) {
            if (line[i] != ' ') {
                crate& c = puzzle.container[(i - 1) / 4];
                c.push_front(line[i]);
            }
        }
    }

    std::regex regex(R"(move (\d+) from (\d+) to (\d+))");
    for (auto it = std::sregex_iterator(input.begin(), input.end(), regex); it != std::sregex_iterator(); ++it) {
        const auto &match = *it;

        puzzle.moves.push_back({ std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3]) });
    }

    return puzzle;
}

void moveSingleCargo(day_t& puzzle, const move& m) {
    crate &source = puzzle.container[m.source - 1];
    crate &target = puzzle.container[m.target - 1];

    int moves { m.amount };
    while (moves--) {
        char c = source.back();
        source.pop_back();
        target.push_back(c);
    }
}

void moveBatchCargo(day_t& puzzle, const move& m) {
    crate &source = puzzle.container[m.source - 1];
    crate &target = puzzle.container[m.target - 1];

    int moves { m.amount };
    auto it = source.end();
    while (moves--) {
        --it;
    }

    target.insert(target.end(), it, source.end());
    source.erase(it, source.end());
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    for (const move& m : input.moves) {
        moveSingleCargo(input, m);
    }

    for (const crate& c : input.container) {
        output << c.back();
    }

    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    for (const move& m : input.moves) {
        moveBatchCargo(input, m);
    }

    for (const crate& c : input.container) {
        output << c.back();
    }

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
