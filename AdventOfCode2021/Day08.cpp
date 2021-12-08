#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <set>

#define TITLE "Day 08"

const uint8_t sig_a = 0b00000001;
const uint8_t sig_b = 0b00000010;
const uint8_t sig_c = 0b00000100;
const uint8_t sig_d = 0b00001000;
const uint8_t sig_e = 0b00010000;
const uint8_t sig_f = 0b00100000;
const uint8_t sig_g = 0b01000000;

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

    size_t sum = 0;

    for (const ScrambledInformation &info : input) {
        // setup decryption
        std::set<uint8_t> bucket[6];
        uint8_t a { 0 }, b { 0 }, c { 0 }, d { 0 }, e { 0 }, f { 0 }, g { 0 };

        for (size_t i = 0; i < 14; ++i) {
            bucket[info.s_input[i] - 2].emplace(info.input[i]);
        }

        uint8_t d_1 = *(bucket[0].begin());
        uint8_t d_7 = *(bucket[1].begin());
        uint8_t d_4 = *(bucket[2].begin());
        uint8_t d_235_s = bucket[3].size();
        uint8_t d_235 = 0;
        for (uint8_t it : bucket[3]) {
            if (d_235 == 0)
                d_235 = it;
            else
                d_235 &= it;
        }
        uint8_t d_069_s = bucket[4].size();
        uint8_t d_069 = 0;
        for (uint8_t it : bucket[4]) {
            if (d_069 == 0)
                d_069 = it;
            else
                d_069 &= it;
        }
        uint8_t d_8 = *(bucket[5].begin());

        // logic
        a = d_1 ^ d_7;
        d = d_4 & d_235;
        e = (d_7 | d_4 | d_069) ^ d_8;
        g = (d_235 & d_069) ^ a;
        c = static_cast<uint8_t>(~(d_069 & d_8) ^ 0b10000000) ^ d ^ e;
        f = d_1 ^ c;
        b = d_4 ^ c ^ d ^ f;

        // decode output
        const uint8_t digit_0 = a | b | c | e | f | g;
        const uint8_t digit_1 = c | f;
        const uint8_t digit_3 = a | c | d | f | g;
        const uint8_t digit_2 = a | c | d | e | g;
        const uint8_t digit_4 = b | c | d | f;
        const uint8_t digit_5 = a | b | d | f | g;
        const uint8_t digit_6 = a | b | d | e | f | g;
        const uint8_t digit_7 = a | c | f;
        const uint8_t digit_8 = a | b | c | d | e | f | g;
        const uint8_t digit_9 = a | b | c | d | f | g;

        uint16_t decoded[4] { 0 };
        for (size_t i = 0; i < 4; i++) {
            uint8_t codedOutput = info.input[10 + i];
            if (codedOutput == digit_0)
                decoded[i] = 0;
            else if (codedOutput == digit_1)
                decoded[i] = 1;
            else if (codedOutput == digit_2)
                decoded[i] = 2;
            else if (codedOutput == digit_3)
                decoded[i] = 3;
            else if (codedOutput == digit_4)
                decoded[i] = 4;
            else if (codedOutput == digit_5)
                decoded[i] = 5;
            else if (codedOutput == digit_6)
                decoded[i] = 6;
            else if (codedOutput == digit_7)
                decoded[i] = 7;
            else if (codedOutput == digit_8)
                decoded[i] = 8;
            else if (codedOutput == digit_9)
                decoded[i] = 9;
            else
                std::cout << "FUCK!" << '\n';
        }

        uint16_t decoded_sum = decoded[0] * 1000 + decoded[1] * 100 + decoded[2] * 10 + decoded[3];
        sum += decoded_sum;
    }

    output << sum;
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
