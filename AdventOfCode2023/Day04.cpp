#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <deque>
#include "parser.cpp"

#define TITLE "Day 04"

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);

struct card {
    short id { 0 };
    int copies { 1 };
    std::vector<short> winningNumbers;
    std::vector<short> myNumbers;
};

typedef std::vector<card> day_t;

day_t parseInput(std::string &input) {
    day_t cards;
    Parser p(input.c_str());

    while (p.consume("Card")) {
        card card;
        p.consumeWhitespace();
        p.readNumber(card.id);
        p.consume(":");

        short s {0};
        while (p.consumeWhitespace() && p.readNumber(s)) {
            card.winningNumbers.push_back(s);
            s = 0;
        }

        p.consume("|");

        while (p.consumeWhitespace() && p.readNumber(s)) {
            card.myNumbers.push_back(s);
            s = 0;
        }

        p.readNewLine();
        cards.push_back(card);
    }

    return cards;
}

bool isWinningNumber(const card &card, short number) {
    bool winning = false;
    for (unsigned int i = 0; !winning && i < card.winningNumbers.size(); i++) {
        winning = card.winningNumbers[i] == number;
    }

    return winning;
}

int countWinningCards(const card &card) {
    int winningCards = 0;
    for (auto& c : card.myNumbers) {
        winningCards += isWinningNumber(card, c);
    }

    return winningCards;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    int score {0};
    for (const card &card : input) {
        int winningCards = countWinningCards(card);

        if (winningCards > 0)
            score += 1 << (winningCards - 1);
    }

    output << score;
    return output.str();
}

std::string runPart2(day_t& cards) {
    std::stringstream output;

    int score = 0;
    for (unsigned int i = 0; i < cards.size(); i++) {
        score += cards[i].copies;
        int winningCards = countWinningCards(cards[i]);

        for (unsigned int j = i + 1; j <= i + winningCards; j++) {
            cards[j].copies += cards[i].copies;
        }
    }

    output << score;
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
