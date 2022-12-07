#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <utility>
#include <vector>
#include <memory>
#include <cassert>
#include <deque>

#define TITLE "Day 07"

struct elven_file {
    std::string name;
    unsigned int size;

    elven_file(std::string name, unsigned int size) : name(std::move(name)), size(size) {}
};

struct elven_dir {
    elven_dir(std::string name) : name(std::move(name)) {}
    elven_dir(std::shared_ptr<elven_dir> parent, std::string name) : parent(std::move(parent)), name(std::move(name)) {}

    size_t size { 0 };
    std::shared_ptr<elven_dir> parent;
    std::string name;
    std::vector<std::shared_ptr<elven_dir>> folders;
    std::vector<std::shared_ptr<elven_file>> files;

    std::shared_ptr<elven_dir> getFolder(const std::string &folderName) const {
        std::shared_ptr<elven_dir> folder;
        for (const std::shared_ptr<elven_dir> &subFolder : folders) {
            if (subFolder->name == folderName) {
                folder = subFolder;
                break;
            }
        }

        return folder;
    }
};

void printStructure(const std::shared_ptr<elven_dir> &folder, int indent = 0) {
    for (int i = 0; i < indent - 1; ++i)
        std::cout << "  ";

    std::cout << "\\ "<< folder->name << " (dir)\n";

    for (const std::shared_ptr<elven_file> &file : folder->files) {
        for (int i = 0; i < indent; ++i)
            std::cout << "  ";

        std::cout << "| " << file->name << " " << file->size << "\n";
    }

    for (const std::shared_ptr<elven_dir> &subFolder : folder->folders) {
        printStructure(subFolder, indent + 1);
    }
}

void computeSizes(const std::shared_ptr<elven_dir> &folder) {
    folder->size = 0;
    for (const std::shared_ptr<elven_file> &file : folder->files) {
        folder->size += file->size;
    }

    for (const std::shared_ptr<elven_dir> &subFolder : folder->folders) {
        computeSizes(subFolder);

        folder->size += subFolder->size;
    }
}

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::shared_ptr<elven_dir> day_t;

day_t parseInput(std::string &input) {
    std::stringstream stream(input);
    std::string line;

    std::shared_ptr<elven_dir> rootDirectory = std::make_shared<elven_dir>("/");
    std::shared_ptr<elven_dir> currentDirectory = rootDirectory;

    while (std::getline(stream, line, '\n')) {
        std::vector<std::string> tokens = tokenize(line, " ");

        if (tokens[0] == "$") {
            if (tokens[1] == "cd" && tokens[2] == "/") {
                // navigate to root dir
                currentDirectory = rootDirectory;
            } else if (tokens[1] == "cd" && tokens[2] == "..") {
                // cd to parent
                currentDirectory = currentDirectory->parent;
            } else if (tokens[1] == "cd") {
                // cd into dir
                std::shared_ptr<elven_dir> folder = currentDirectory->getFolder(tokens[2]);
                assert(folder);
                currentDirectory = folder;
            } else if (tokens[1] == "ls") {
                // I assume this is the default mode
            }
        } else {
            if (tokens[0] == "dir") {
                std::shared_ptr<elven_dir> subFolder = std::make_shared<elven_dir>(currentDirectory, tokens[1]);
                currentDirectory->folders.push_back(subFolder);
            } else {
                std::shared_ptr<elven_file> file = std::make_shared<elven_file>(tokens[1], std::stoi(tokens[0]));
                currentDirectory->files.push_back(file);
            }
        }
    }

    return rootDirectory;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    computeSizes(input);

    size_t size { 0 };
    std::deque<std::shared_ptr<elven_dir>> q;
    q.push_back(input);
    while (!q.empty()) {
        const std::shared_ptr<elven_dir>& c = q.front();
        if (c->size <= 100000)
            size += c->size;

        q.insert(q.end(), c->folders.begin(), c->folders.end());
        q.pop_front();
    }

    output << size;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    computeSizes(input);
    size_t requiredSpace = 30000000L - (70000000L - input->size);
    std::deque<std::shared_ptr<elven_dir>> q;
    q.push_back(input);

    std::shared_ptr<elven_dir> deletionCandidate = input;
    while (!q.empty()) {
        std::shared_ptr<elven_dir> &c = q.front();

        if (requiredSpace < c->size && c->size < deletionCandidate->size)
            deletionCandidate = c;

        q.insert(q.end(), c->folders.begin(), c->folders.end());
        q.pop_front();
    }

    output << deletionCandidate->size;
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
