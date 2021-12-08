#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>

#define TITLE "Day 08"

const uint8_t sig_a = 0b00000001;
const uint8_t sig_b = 0b00000010;
const uint8_t sig_c = 0b00000100;
const uint8_t sig_d = 0b00001000;
const uint8_t sig_e = 0b00010000;
const uint8_t sig_f = 0b00100000;
const uint8_t sig_g = 0b01000000;

const uint8_t digit_0 = sig_a | sig_b | sig_c | sig_e | sig_f | sig_g;
const uint8_t digit_1 = sig_c | sig_f;
const uint8_t digit_2 = sig_a | sig_c | sig_d | sig_e | sig_g;
const uint8_t digit_3 = sig_a | sig_c | sig_d | sig_f | sig_g;
const uint8_t digit_4 = sig_b | sig_c | sig_d | sig_f;
const uint8_t digit_5 = sig_a | sig_b | sig_d | sig_f | sig_g;
const uint8_t digit_6 = sig_a | sig_b | sig_d | sig_e | sig_f | sig_g;
const uint8_t digit_7 = sig_a | sig_c | sig_f;
const uint8_t digit_8 = sig_a | sig_b | sig_c | sig_d | sig_e | sig_f | sig_g;
const uint8_t digit_9 = sig_a | sig_b | sig_c | sig_d | sig_f | sig_g;;

struct ScrambledInformation {
    uint8_t input[14] { 0 };
    uint8_t s_input[14] { 0 };
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<ScrambledInformation> day_t;

day_t parseInput(std::string &input) {
    day_t parsed;
    std::stringstream stream(input);

    std::string word;
    int i { 0 };
    ScrambledInformation info;
    while (stream >> word) {
        if (i == 10) {
            // ignore |
            i += 1;
            continue;
        }

        uint8_t state { 0 };
        for (char c : word) {
            switch (c) {
                case 'a':
                    state |= sig_a;
                    break;
                case 'b':
                    state |= sig_b;
                    break;
                case 'c':
                    state |= sig_c;
                    break;
                case 'd':
                    state |= sig_d;
                    break;
                case 'e':
                    state |= sig_e;
                    break;
                case 'f':
                    state |= sig_f;
                    break;
                case 'g':
                    state |= sig_g;
                    break;
                default:
                    break;
            }
        }

        if (i < 10) {
            info.input[i] = state;
            info.s_input[i] = word.size();
        } else {
            info.input[i - 1] = state;
            info.s_input[i - 1] = word.size();
        }

        if (i == 14) {
            parsed.push_back(info);
            i = 0;
        } else {
            i += 1;
        }
    }

    return parsed;
}

std::string runPart1(day_t& input) {
//    std::cout << "0: " << (int) digit_0 << '\n';
//    std::cout << "1: " << (int) digit_1 << '\n';
//    std::cout << "2: " << (int) digit_2 << '\n';
//    std::cout << "3: " << (int) digit_3 << '\n';
//    std::cout << "4: " << (int) digit_4 << '\n';
//    std::cout << "5: " << (int) digit_5 << '\n';
//    std::cout << "6: " << (int) digit_6 << '\n';
//    std::cout << "7: " << (int) digit_7 << '\n';
//    std::cout << "8: " << (int) digit_8 << '\n';
//    std::cout << "9: " << (int) digit_9 << std::endl;

    std::stringstream output;

    size_t appearances { 0 };

    for (const ScrambledInformation &info : input) {
        for (size_t i = 0; i < 4; i++) {
            switch (info.s_input[10 + i]) {
                case 2:
                case 3:
                case 4:
                case 7:
                    appearances += 1;
                    break;
                default:
                    break;
            }
        }
    }

    output << appearances;
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
