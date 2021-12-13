#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <map>
#include <deque>

#define TITLE "Day 12"

struct Cave {
    std::string name;
    bool small { false };

    bool operator<(const Cave &other) const {
        return name < other.name;
    }
};

struct CaveSystem {
    std::map<std::string, Cave> caves;
    std::map<std::string, std::vector<Cave>> connections;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef CaveSystem day_t;

day_t parseInput(std::string &input) {
    day_t parsed;

    std::stringstream stream(input);
    std::string line;
    while (stream >> line) {
        size_t idx = line.find('-');
        std::string cave_from = line.substr(0, idx);
        std::string cave_to = line.substr(idx + 1);

        Cave from;
        from.name = cave_from;
        from.small = cave_from.find_first_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos;

        Cave to;
        to.name = cave_to;
        to.small = cave_to.find_first_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos;

        parsed.caves[from.name] = from;
        parsed.caves[to.name] = to;
        parsed.connections[from.name].push_back(to);
        parsed.connections[to.name].push_back(from);
    }

    return parsed;
}

struct node {
    const node* previous {nullptr};
    std::string name;
    bool small;
};

bool has_visited(const node* n, const std::string &name) {
    while (n != nullptr) {
        if (n->name == name)
            return true;

        n = n->previous;
    }

    return false;
}

#ifdef DEBUG
void print_path(const node* n) {
    if (n->previous != nullptr)
        print_path(n->previous);

    std::cout << " " << n->name;
}
#endif

std::string runPart1(day_t& input) {
    std::stringstream output;
    std::deque<const node*> allocated_nodes;
    std::deque<const node*> complete_paths;

    std::deque<const node*> queue;
    node* start_node = new node();
    start_node->name = "start";
    allocated_nodes.push_back(start_node);
    queue.push_back(start_node);

    while (!queue.empty()) {
        const node* c = queue.front();
        queue.pop_front();

        const std::vector<Cave>& next_caves = input.connections[c->name];
        for (const Cave& next_cave : next_caves) {
            if (next_cave.small && has_visited(c, next_cave.name))
                continue;

            node* n = new node();
            n->name = next_cave.name;
            n->previous = c;
            allocated_nodes.push_back(n);

            if ("end" == next_cave.name)
                complete_paths.push_back(n);
            else
                queue.push_back(n);
        }
    }

    output << complete_paths.size();

    // don't leek memory even though we could
    for (const node *n : allocated_nodes) {
        delete n;
    }

    return output.str();
}

bool can_visit_small_cave(const node* c, const std::string& name) {
    std::map<std::string, uint16_t> visits;
    while (c != nullptr) {
        if (c->small)
            visits[c->name] += 1;

        c = c->previous;
    }

    bool visitedTwice = false;
    for (const auto &it : visits) {
        if (it.first != name && it.second >= 2)
            visitedTwice = true;
    }

    return !visitedTwice && visits[name] < 2 || visitedTwice && visits[name] < 1;
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    std::deque<const node*> allocated_nodes;
    std::deque<const node*> complete_paths;

    std::deque<const node*> queue;
    node* start_node = new node();
    start_node->name = "start";
    allocated_nodes.push_back(start_node);
    queue.push_back(start_node);

    while (!queue.empty()) {
        const node* c = queue.front();
        queue.pop_front();

        const std::vector<Cave>& next_caves = input.connections[c->name];
        for (const Cave& next_cave : next_caves) {
            if (next_cave.small && !can_visit_small_cave(c, next_cave.name) || next_cave.name == "start")
                continue;

            node* n = new node();
            n->name = next_cave.name;
            n->small = next_cave.small;
            n->previous = c;
            allocated_nodes.push_back(n);

            if ("end" == next_cave.name)
                complete_paths.push_back(n);
            else
                queue.push_back(n);
        }
    }

#ifdef DEBUG
    for (const node* path : complete_paths) {
        print_path(path);
        std::cout << std::endl;
    }
#endif

    output << complete_paths.size();

    // don't leek memory even though we could
    for (const node *n : allocated_nodes) {
        delete n;
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
