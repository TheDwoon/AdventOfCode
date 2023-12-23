#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include "parser.cpp"

#define TITLE "Day 19"

struct material {
    int x {0};
    int m {0};
    int a {0};
    int s {0};
};

struct workflow_rule {
    char variable{};
    char operation{};
    int threshold {0};
    std::string action;
};

struct workflow {
    std::string name;
    std::vector<workflow_rule> rules;
    std::vector<material> materials;
};

struct req {
    std::vector<workflow> workflows;
    std::vector<material> materials;
};

typedef req day_t;

workflow* getWorkflow(std::vector<workflow> &wfs, const std::string &name) {
    for (auto& wf : wfs) {
        if (wf.name == name)
            return &wf;
    }

    throw std::exception();
}

int getMaterialValue(char variable, const material &m) {
    switch (variable) {
        case 'x': return m.x;
        case 'm': return m.m;
        case 'a': return m.a;
        case 's': return m.s;
        default: assert(false); return 0;
    }
}

std::string applyWorkflow(const std::vector<workflow_rule> &rules, const material &m) {
    for (int i = 0; i < rules.size() - 1; i++) {
        auto &r = rules[i];
        if (r.operation == '<'  && getMaterialValue(r.variable, m) < r.threshold) {
            return r.action;
        } else if (r.operation == '>' && getMaterialValue(r.variable, m) > r.threshold) {
            return r.action;
        }
    }

    return rules[rules.size() - 1].action;
}

void applyWorkflow(std::vector<workflow> &wfs, const material &m) {
    workflow* wf = getWorkflow(wfs, "in");
    while (!wf->rules.empty()) {
        std::string next = applyWorkflow(wf->rules, m);
        std::cout << "[" << wf->name << "] => [" << next << "]" << std::endl;
        wf = getWorkflow(wfs, next);
    }

    std::cout << "*************" << std::endl;

    wf->materials.push_back(m);
}

void parseWorkflowRules(Parser &p, workflow &wf) {
    while (p.peek() != '}') {
        workflow_rule wfr;
        unsigned int offset;
        ;
        if (p.peek() >= 'a' && p.peek() <= 'z' && (p.getCurrent()[1] == '<' || p.getCurrent()[1]== '>')) {
            // has a condition
            wfr.variable = p.read();
            wfr.operation = p.read();
            p.readNumber(wfr.threshold);
            p.consume(':');

            p.findNext(",", offset);
            p.readString(wfr.action, offset);
            p.consume(',');
        } else {
            p.consume(',');
            // is default action
            p.findNext("}", offset);
            p.readString(wfr.action, offset);
        }

        wf.rules.push_back(wfr);
    }
}

day_t parseInput(std::string &input) {
    day_t result;
    Parser p(input.c_str());
    while (!p.isNewLine()) {
        workflow wf;
        unsigned int offset;
        p.findNext("{", offset);
        p.readString(wf.name, offset);
        p.consume('{');
        parseWorkflowRules(p, wf);
        p.consume('}');
        p.readNewLine();

        result.workflows.push_back(wf);
    }

    p.readNewLine();

    while (!p.eof()) {
        material m;
        p.consume("{x=");
        p.readNumber(m.x);
        p.consume(",m=");
        p.readNumber(m.m);
        p.consume(",a=");
        p.readNumber(m.a);
        p.consume(",s=");
        p.readNumber(m.s);
        p.consume("}");
        p.readNewLine();

        result.materials.push_back(m);
    }

    result.workflows.emplace_back("A");
    result.workflows.emplace_back("R");

    return result;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    int score = 0;
    for (auto &m : input.materials) {
        applyWorkflow(input.workflows, m);
    }

    workflow* wf = getWorkflow(input.workflows, "A");
    for (const material &m : wf->materials) {
        score += m.x + m.m + m.a + m.s;
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
