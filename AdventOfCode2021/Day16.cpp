#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>

#define TITLE "Day 16"

struct bits_transmission {
    std::vector<uint8_t> data;
    uint64_t bits { 0 };
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef bits_transmission day_t;

template<typename T>
T read_bits(uint32_t start_bit, uint16_t length, const bits_transmission &input) {
    T result { 0 };
    const uint8_t* raw_data = input.data.data();

    size_t block_offset = start_bit % 8;
    size_t start_idx = start_bit / 8;
    size_t end_idx = (start_bit + length + 7) / 8;
    size_t block_size = end_idx - start_idx;
    switch (block_size) {
        case 1: {
            result = static_cast<T>(raw_data[start_idx] << block_offset);
            break;
        }
        case 2: {
            const auto* d = reinterpret_cast<const uint16_t*>(raw_data + start_idx);
            result = static_cast<T>(*d << block_offset);
            break;
        }
        case 3:
        case 4: {
            const auto* d = reinterpret_cast<const uint32_t *>(raw_data + start_idx);
            result = static_cast<T>(*d << block_offset);
            break;
        }
        case 5:
        case 6:
        case 7:
        case 8: {
            const auto* d = reinterpret_cast<const uint64_t*>(raw_data + start_idx);
            result = static_cast<T>(*d << block_offset);
            break;
        }
    }

    size_t type_size = sizeof(T) * 8;
    result = result >> (type_size - length);
    return result;
}

struct base_packet {
    uint8_t version { 0 };
    uint8_t type { 0 };
};

struct literal_packet : public base_packet {
    uint64_t literal { 0 };
};

struct operator_packet : public base_packet {
    uint8_t length_type { 0 };
    std::vector<base_packet*> packets;
};

literal_packet* read_literal() {
    return nullptr;
}

operator_packet* read_operator() {
    return nullptr;
}

day_t parseInput(std::string &input) {
    day_t parsed;

    uint8_t block_count {0 };
    uint8_t block { 0 };
    for (char c : input) {
        if (c == '\n')
            continue;

        block = block << 4;
        switch (c) {
            case '0': block = block | 0b0000; break;
            case '1': block = block | 0b0001; break;
            case '2': block = block | 0b0010; break;
            case '3': block = block | 0b0011; break;
            case '4': block = block | 0b0100; break;
            case '5': block = block | 0b0101; break;
            case '6': block = block | 0b0110; break;
            case '7': block = block | 0b0111; break;
            case '8': block = block | 0b1000; break;
            case '9': block = block | 0b1001; break;
            case 'A': block = block | 0b1010; break;
            case 'B': block = block | 0b1011; break;
            case 'C': block = block | 0b1100; break;
            case 'D': block = block | 0b1101; break;
            case 'E': block = block | 0b1110; break;
            case 'F': block = block | 0b1111; break;
        }

        if (++block_count % 2 == 0) {
            parsed.data.push_back(block);
            block = 0;
        }
    }

    if (block_count % 2 != 0)
        parsed.data.push_back(block);

    parsed.bits = block_count * 4;

    return parsed;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    auto b = read_bits<uint8_t>(0, 6, input);

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
