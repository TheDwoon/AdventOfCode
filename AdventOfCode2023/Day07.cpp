#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "parser.cpp"

#define TITLE "Day 07"
#define ONE_PAIR 0
#define TWO_OF_A_KIND 1
#define THREE_OF_A_KIND 2
#define FOUR_OF_A_KIND 3
#define FIVE_OF_A_KIND 4

#define GRADE_FIVE_OF_A_KIND 6
#define GRADE_FOUR_OF_A_KIND 5
#define GRADE_FULL_HOUSE 4
#define GRADE_THREE_OF_A_KIND 3
#define GRADE_TWO_PAIRS 2
#define GRADE_ONE_PAIR 1
#define GRADE_HIGH_CARD 0

struct hand {
    std::array<char, 5> card;
    int bid;

    std::array<char, 5> pairs;
    char grade;
    char jokers;
};

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

char numberToCard(char c) {
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
        case 1: return 'J';
        default: return c;
    }
}

day_t parseInput(std::string &input) {
    std::vector<hand> result;
    Parser p(input.c_str());

    while (!p.eof()) {
        hand h {};
        h.card[0] = cardToNumber(p.read());
        h.card[1] = cardToNumber(p.read());
        h.card[2] = cardToNumber(p.read());
        h.card[3] = cardToNumber(p.read());
        h.card[4] = cardToNumber(p.read());

        assert(h.card[0] >= 2 && h.card[0] <= 14);
        assert(h.card[1] >= 2 && h.card[1] <= 14);
        assert(h.card[2] >= 2 && h.card[2] <= 14);
        assert(h.card[3] >= 2 && h.card[3] <= 14);
        assert(h.card[4] >= 2 && h.card[4] <= 14);

        p.consume(' ');
        p.readNumber(h.bid);
        p.readNewLine();
        result.push_back(h);
    }

    return result;
}

bool hasFiveOfAKind(const hand& h) {
    for (char j = 0; j <= FIVE_OF_A_KIND && j <= h.jokers; j++) {
        if (h.pairs[FIVE_OF_A_KIND - j] >= 1)
            return true;
    }

    return 5 <= h.jokers;
}

bool hasFourOfAKind(const hand& h) {
    for (char j = 0; j <= FOUR_OF_A_KIND && j <= h.jokers; j++) {
        if (h.pairs[FOUR_OF_A_KIND - j] >= 1)
            return true;
    }

    return 4 <= h.jokers;
}

bool hasThreeOfAKind(const hand& h) {
    for (char j = 0; j <= THREE_OF_A_KIND && j <= h.jokers; j++) {
        if (h.pairs[THREE_OF_A_KIND - j] >= 1)
            return true;
    }

    return 3 <= h.jokers;
}

void hasThreeOfAKind(std::array<char, 5> &pairs, char &jokers) {
    for (char j = 0; j <= THREE_OF_A_KIND; j++) {
        if (pairs[THREE_OF_A_KIND - j] >= 1) {
            pairs[THREE_OF_A_KIND - j] -= 1;
            jokers += j;
            return;
        }
    }

    jokers += 3;
}

void hasTwoOfAKind(std::array<char, 5> &pairs, char &jokers) {
    for (char j = 0; j <= TWO_OF_A_KIND; j++) {
        if (pairs[TWO_OF_A_KIND - j] >= 1) {
            pairs[TWO_OF_A_KIND - j] -= 1;
            jokers += j;
            return;
        }
    }

    jokers += 2;
}

bool hasTwoPairs(const hand& h) {
    std::array<char, 5> pairs = h.pairs;
    char jokers = 0;
    hasTwoOfAKind(pairs, jokers);
    hasTwoOfAKind(pairs, jokers);
    return jokers <= h.jokers;
}

bool hasFullHouse(const hand& h) {
    std::array<char, 5> pairs = h.pairs;
    char jokers = 0;
    hasThreeOfAKind(pairs, jokers);
    hasTwoOfAKind(pairs, jokers);
    return jokers <= h.jokers;
}

bool hasOnePair(const hand& h) {
    for (char j = 0; j <= TWO_OF_A_KIND && j <= h.jokers; j++) {
        if (h.pairs[TWO_OF_A_KIND - j] >= 1) {
            return true;
        }
    }

    return false;
}

void gradeHand(hand& hand) {
    char cards[5] = {hand.card[0], hand.card[1], hand.card[2], hand.card[3], hand.card[4] };
    std::sort(cards, cards + 5);

    hand.jokers = 0;
    while (hand.jokers < 5 && cards[hand.jokers] == 1)
        hand.jokers++;

    char i = hand.jokers;
    while (i < 5) {
        unsigned int count = 0;
        while (i < 4 && cards[i] == cards[i + 1]) {
            i++;
            count++;
        }

        assert(count < 5);
        hand.pairs[count] += 1;
        i++;
    }

    if (hasFiveOfAKind(hand)) hand.grade = GRADE_FIVE_OF_A_KIND;
    else if (hasFourOfAKind(hand)) hand.grade = GRADE_FOUR_OF_A_KIND;
    else if (hasFullHouse(hand)) hand.grade = GRADE_FULL_HOUSE;
    else if (hasThreeOfAKind(hand)) hand.grade = GRADE_THREE_OF_A_KIND;
    else if (hasTwoPairs(hand)) hand.grade = GRADE_TWO_PAIRS;
    else if (hasOnePair(hand)) hand.grade = GRADE_ONE_PAIR;
}

bool compareHands(const hand& a, const hand& b) {
    if (a.grade < b.grade) {
        return true;
    } else if (a.grade == b.grade) {
        for (unsigned int i = 0; i < 5; i++) {
            if (a.card[i] < b.card[i])
                return true;
            else if (a.card[i] > b.card[i])
                return false;
        }

        return false;
    } else {
        return false;
    }
}

void printGrade(char grade) {
    switch (grade) {
        case GRADE_FIVE_OF_A_KIND: std::cout << "FIVE_OF_A_KIND "; break;
        case GRADE_FOUR_OF_A_KIND: std::cout << "FOUR_OF_A_KIND "; break;
        case GRADE_FULL_HOUSE: std::cout << "FULL_HOUSE     "; break;
        case GRADE_THREE_OF_A_KIND: std::cout << "THREE_OF_A_KIND"; break;
        case GRADE_TWO_PAIRS: std::cout << "TWO_PAIRS      "; break;
        case GRADE_ONE_PAIR: std::cout << "ONE_PAIR       "; break;
        case GRADE_HIGH_CARD: std::cout << "HIGH_CARD      "; break;
        default: std::cout << "???            "; break;
    }
}

void printHand(const hand& h) {
    std::cout << "Hand: [";
    for (char c : h.card)
        std::cout << numberToCard(c);
    std::cout << "] Pairs: [";
    for (char c : h.pairs)
        std::cout << (int) c << ", ";
    std::cout << "] grade: ";
    printGrade(h.grade);
    std::cout << " jokers: " << (int) h.jokers << std::endl;
}

void printHand(const day_t &hands) {
    for (const hand &h : hands)
        printHand(h);
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    uint64_t score = 0;

    for (hand& h : input) {
        gradeHand(h);
    }

    std::sort(input.begin(), input.end(), compareHands);
//    printHand(input);

    for (unsigned int i = 0; i < input.size(); i++) {
        score += static_cast<uint64_t>(i + 1) * static_cast<uint64_t>(input[i].bid);
     }

    output << score;
    return output.str();
}

void convertJokers(day_t& hands) {
    char j = cardToNumber('J');
    for (hand &h : hands) {
        for (char &c : h.card) {
            if (c == j)
                c = 1;
        }
    }
}

std::string runPart2(day_t& input) {
    convertJokers(input);
    return runPart1(input);
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
