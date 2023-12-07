#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include "parser.cpp"

#define TITLE "Day 07"
#define ONE_PAIR 0
#define TWO_OF_A_KIND 1
#define THREE_OF_A_KIND 2
#define FOUR_OF_A_KIND 3
#define ONE_OF_A_KIND 4

struct hand {
    char card[5] {0};
    int bid {0};

    char pairs[5] {0};
    char highestCard {0};
    char grade{0};
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<hand> day_t;

char cardToNumber(char c) {
    switch (c) {
        case 'A': return 14;
        case 'K': return 13;
        case 'Q': return 12;
        case 'J': return 11;
        case 'T': return 10;
        case '9': return 9;
        case '8': return 8;
        case '7': return 7;
        case '6': return 6;
        case '5': return 5;
        case '4': return 4;
        case '3': return 3;
        case '2': return 2;
        default: return c;
    }
}

char NumberToCard(char c) {
    switch (c) {
        case 14: return 'A';
        case 13: return 'K';
        case 12: return 'Q';
        case 11: return 'J';
        case 10: return 'T';
        case 9: return '9';
        case 8: return '8';
        case 7: return '7';
        case 6: return '6';
        case 5: return '5';
        case 4: return '4';
        case 3: return '3';
        case 2: return '2';
        default: return c;
    }
}

day_t parseInput(std::string &input) {
    std::vector<hand> result;
    Parser p(input.c_str());

    while (!p.eof()) {
        hand h;
        h.card[0] = cardToNumber(p.read());
        h.card[1] = cardToNumber(p.read());
        h.card[2] = cardToNumber(p.read());
        h.card[3] = cardToNumber(p.read());
        h.card[4] = cardToNumber(p.read());

        p.consume(' ');
        p.readNumber(h.bid);
        p.readNewLine();
        std::cout << sizeof(h) << "Hellooo" << std::endl;
        result.push_back(h);
    }

    return result;
}

bool isFiveOfAKind(const hand& h) {
    return h.pairs[ONE_OF_A_KIND] == 1;
}

bool isFourOfAKind(const hand& h) {
    return h.pairs[FOUR_OF_A_KIND] == 1;
}

bool isFullHouse(const hand& h) {
    return h.pairs[THREE_OF_A_KIND] == 1 && h.pairs[TWO_OF_A_KIND] == 1;
}

bool isThreeOfAKind(const hand& h) {
    return h.pairs[THREE_OF_A_KIND] == 1 && h.pairs[TWO_OF_A_KIND] == 0;
}

bool isTwoPair(const hand& h) {
    return h.pairs[TWO_OF_A_KIND] == 2;
}

bool isOnePair(const hand& h) {
    return h.pairs[TWO_OF_A_KIND] == 1 && h.pairs[THREE_OF_A_KIND] == 0;
}

void gradeHand(hand& hand) {
    char cards[5] = {hand.card[0], hand.card[1], hand.card[2], hand.card[3], hand.card[4] };
    std::sort(cards, cards + 5);
    hand.highestCard = cards[4];

    unsigned int i = 0;
    while (i < 5) {
        unsigned int count = 0;
        while (i < 4 && cards[i] == cards[i + 1]) {
            i++;
            count++;
        }

        hand.pairs[count] += 1;
        i++;
    }

    if (isOnePair(hand)) hand.grade = 1;
    if (isTwoPair(hand)) hand.grade |= 2;
    if (isThreeOfAKind(hand)) hand.grade |= 4;
    if (isFullHouse(hand)) hand.grade |= 8;
    if (isFourOfAKind(hand)) hand.grade |= 16;
    if (isFiveOfAKind(hand)) hand.grade |= 32;

    assert(hand.grade == 0 || hand.grade == 1 || hand.grade == 2 || hand.grade == 4 || hand.grade == 8 || hand.grade == 16 || hand.grade == 32);
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    uint64_t score = 0;

    for (hand& h : input) {
        gradeHand(h);
    }

    std::sort(input.begin(), input.end(), [](const hand& a, const hand& b) {
        if (a.grade < b.grade) {
            return true;
        } else if (a.grade == b.grade) {
            for (unsigned int i = 0; i < 5; i++) {
                if (a.card[i] < b.card[i])
                    return true;
            }

            return false;
        } else {
            return false;
        }
    });

    for (unsigned int i = 0; i < input.size(); i++) {
        score += static_cast<uint64_t>(i + 1) * static_cast<uint64_t>(input[i].bid);
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
