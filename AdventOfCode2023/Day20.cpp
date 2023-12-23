#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <queue>
#include <map>
#include "parser.cpp"

#define TITLE "Day 20"

enum module_type {
    NONE,
    BROADCAST,
    FLIP_FLOP,
    CONJUNCTION
};

enum pulse {
    LOW,
    HIGH
};

struct module {
    std::string name;
    module_type type { NONE };
    std::vector<std::string> input_names;
    std::vector<pulse> input_pulse;
    std::vector<std::string> outputs;
    bool is_on { false } ;
};

struct inflight_signal {
    std::string source;
    std::string target;
    pulse pulse;
};

typedef std::map<std::string, module> day_t;

day_t parseInput(std::string &input) {
    day_t modules;
    Parser p(input.c_str());
    while (!p.eof()) {
        module m;
        if (p.consume('%')) {
            m.type = FLIP_FLOP;
        } else if (p.consume('&')) {
            m.type = CONJUNCTION;
        } else {
            m.type = BROADCAST;
        }
        unsigned int offset;
        p.findNext(" -> ", offset);
        p.readString(m.name, offset);
        p.consume(" -> ");

        while (!p.isNewLine() && (p.findNext(", ", offset) || p.findNext("\r\n", offset) || p.findNext("\n", offset))) {
            std::string output;
            p.readString(output, offset);
            m.outputs.push_back(output);
            p.consume(", ");
        }

        modules[m.name] = m;
        p.readNewLine();
    }

    for (auto& it : modules) {
        for (const std::string &output : it.second.outputs) {
            modules[output].input_names.push_back(it.first);
        }
    }

    for (auto& it : modules) {
        it.second.input_pulse.resize(it.second.input_names.size());
    }

    return modules;
}

int getInputIndex(const module &m, const std::string &source) {
    for (int i = 0; i < m.input_names.size(); i++) {
        if (m.input_names[i] == source)
            return i;
    }

    return -1;
}

std::string runPart1(day_t& modules) {
    std::stringstream output;
    std::queue<inflight_signal> signal_queue;

    inflight_signal is { "button", "broadcaster", LOW };
    signal_queue.push(is);

    while (!signal_queue.empty()) {
        is = signal_queue.front();
        signal_queue.pop();

        std::cout << is.source << " -[" << is.pulse << "]-> " << is.target << std::endl;

        module &m = modules[is.target];
        if (m.type == BROADCAST) {
            for (const std::string &o : m.outputs) {
                signal_queue.emplace(m.name, o, is.pulse);
            }
        } else if (m.type == FLIP_FLOP) {
            if (is.pulse == LOW) {
                m.is_on = !m.is_on;
                if (m.is_on) {
                    for (const std::string &o : m.outputs) {
                        signal_queue.emplace(m.name, o, HIGH);
                    }
                } else {
                    for (const std::string &o : m.outputs) {
                        signal_queue.emplace(m.name, o, LOW);
                    }
                }
            }
        } else if (m.type == CONJUNCTION) {
            int signalIndex = getInputIndex(m, is.source);
            assert(signalIndex >= 0);
            m.input_pulse[signalIndex] = is.pulse;
            bool allInputsHigh = true;
            for (pulse p : m.input_pulse) {
                if (p == LOW) {
                    allInputsHigh = false;
                    break;
                }
            }

            for (const std::string &o : m.outputs) {
                signal_queue.emplace(m.name, o, allInputsHigh ? LOW : HIGH);
            }
        } else {
            assert(false);
        }
    }

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
