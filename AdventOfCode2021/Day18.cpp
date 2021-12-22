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

#define TITLE "Day 18"
#define DEBUG

enum side_t { ROOT, LEFT, RIGHT };

struct snailfish_number {
    std::shared_ptr<snailfish_number> parent;
    side_t side {ROOT };

    std::shared_ptr<snailfish_number> left;
    uint16_t left_number;

    std::shared_ptr<snailfish_number> right;
    uint16_t right_number;

    bool hasLeftTree() const {
        if (left)
            return true;
        else
            return false;
    }

    bool hasRightTree() const {
        if (right)
            return true;
        else
            return false;
    }
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<std::shared_ptr<snailfish_number>> day_t;
typedef std::shared_ptr<snailfish_number> snailfish_ptr;

#ifdef DEBUG
void print_snailfish(const snailfish_ptr &ptr) {
    if (!ptr) {
        std::cout << "[null]";
        return;
    }

    std::cout << '[';
    if (ptr->hasLeftTree())
        print_snailfish(ptr->left);
    else
        std::cout << ptr->left_number;

    std::cout << ',';
    if (ptr->hasRightTree())
        print_snailfish(ptr->right);
    else
        std::cout << ptr->right_number;

    std::cout << ']';
}
#endif

std::shared_ptr<snailfish_number> parse_number(std::stringstream &stream) {
    snailfish_ptr ptr = std::make_shared<snailfish_number>();
    char c;

    // parse left side_t
    stream.get(c);
    if (c >= '0' && c <= '9') {
        ptr->left_number = c - '0';
    } else if (c == '[') {
        ptr->left = parse_number(stream);
        ptr->left->parent = ptr;
        ptr->left->side = LEFT;
    }

    // expect comma here
    stream.get(c);
    assert(c == ',');

    // parse right side_t
    stream.get(c);
    if (c >= '0' && c <= '9') {
        ptr->right_number = c - '0';
    } else if (c == '[') {
        ptr->right = parse_number(stream);
        ptr->right->parent = ptr;
        ptr->right->side = RIGHT;
    }

    // expect closing bracket
    stream.get(c);
    assert(c == ']');

    return ptr;
}

day_t parseInput(std::string &input) {
    day_t parsed;
//    input = "[[[[[9,8],1],2],3],4]\n[7,[6,[5,[4,[3,2]]]]]\n[[6,[5,[4,[3,2]]]],1]\n[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]\n[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]\n";
    input = "[[[[4,3],4],4],[7,[[8,4],9]]]\n[1,1]\n";
    std::stringstream stream(input);

    char c;
    while (stream) {
        if (!stream.get(c))
            break;

        assert(c == '[');
        snailfish_ptr ptr = parse_number(stream);
        parsed.push_back(std::move(ptr));

        if (!stream.get(c))
            break;

        assert(c == '\n');
    }

    return parsed;
}

size_t snailfish_depth(const snailfish_ptr &ptr) {
    if (!ptr)
        return 0;

    return 1 + std::max(snailfish_depth(ptr->left), snailfish_depth(ptr->right));
}

bool snailfish_explode(snailfish_ptr &root) {
    size_t depth = snailfish_depth(root);

    // nothing to explode (root nodes counts towards depth)
    if (depth < 5)
        return false;

    int current_depth = 1;
    snailfish_ptr explode_target = root;
    while (explode_target->hasRightTree() || explode_target->hasLeftTree()) {
        size_t left_depth = snailfish_depth(explode_target->left);
        size_t right_depth = snailfish_depth(explode_target->right);
        if (left_depth >= right_depth)
            explode_target = explode_target->left;
        else
            explode_target = explode_target->right;
    }

#ifdef DEBUG
    // target for exploding
    print_snailfish(explode_target);
    std::cout << '\n';
#endif
    assert(explode_target->side == LEFT || explode_target->side == RIGHT);
    assert(explode_target && !explode_target->hasLeftTree() && !explode_target->hasRightTree());

    uint16_t* target_left{nullptr};
    {
        // move up until we have the option to go left (not ascending from the left node)
        snailfish_ptr c = explode_target;
        while (c && (c->side == LEFT || c->side == ROOT))
            c = c->parent;

        if (c && c->side == RIGHT) {
            c = c->parent;
        }

        if (c && c->hasLeftTree()) {
            c = c->left;
            while (c->hasRightTree())
                c = c->right;

            target_left = &(c->right_number);
        } else if (c) {
            target_left = &(c->left_number);
        }
    }
#ifdef DEBUG
    if (target_left) {
        std::cout << "Left target found: " << *target_left << '\n';
    } else {
        std::cout << "No left target found!\n";
    }
#endif

    uint16_t* target_right {nullptr};
    {
        // move up until we have the option to move right (not ascending from right node)
        snailfish_ptr c = explode_target;
        while (c && (c->side == RIGHT || c->side == ROOT))
            c = c->parent;

        if (c && c->side == LEFT) {
            c = c->parent;
        }

        if (c && c->hasRightTree()) {
            c = c->right;
            while (c->hasLeftTree())
                c = c->left;

            target_right = &(c->left_number);
        } else if (c) {
            target_right = &(c->right_number);
        }
    }
#ifdef DEBUG
    if (target_right) {
        std::cout << "Right target found: " << *target_right << '\n';
    } else {
        std::cout << "No right target found!\n";
    }
#endif

    // explode node
    if (target_right)
        *target_right += explode_target->right_number;

    if (target_left)
        *target_left += explode_target->left_number;

    if (explode_target->side == LEFT) {
        explode_target->parent->left = snailfish_ptr();
        explode_target->parent->left_number = 0;
    } else {
        explode_target->parent->right = snailfish_ptr();
        explode_target->parent->right_number = 0;
    }

#ifdef DEBUG
    std::cout << "After explode: ";
    print_snailfish(root);
    std::cout << '\n';
#endif

    return true;
}

bool snailfish_split(std::shared_ptr<snailfish_number> &ptr) {
    bool result {false};

    if (ptr->left_number > 9) {
        assert(!ptr->left);
        snailfish_ptr split = std::make_shared<snailfish_number>();
        split->side = LEFT;
        split->parent = ptr;
        split->left_number = ptr->left_number / 2;
        split->right_number = (ptr->left_number + 1) / 2;
        ptr->left_number = 0;
        ptr->left = split;
        return true;
    } else if (ptr->left) {
        result = snailfish_split(ptr->left);
    }

    if (ptr->right_number > 9) {
        assert(!ptr->right);
        snailfish_ptr split = std::make_shared<snailfish_number>();
        split->side = RIGHT;
        split->parent = ptr;
        split->left_number = ptr->right_number / 2;
        split->right_number = (ptr->right_number + 1) / 2;
        ptr->right_number = 0;
        ptr->right = split;
        return true;
    } else if (!result && ptr->right) {
        result = snailfish_split(ptr->right);
    }

    return result;
}

void snailfish_reduce(std::shared_ptr<snailfish_number> &root) {
    while (snailfish_explode(root) || snailfish_split(root)) {
#ifdef DEBUG
        std::cout << " => ";
        print_snailfish(root);
        std::cout << '\n';
#endif
    }
}

std::shared_ptr<snailfish_number> snailfish_add(std::shared_ptr<snailfish_number> left, std::shared_ptr<snailfish_number> right) {
    std::shared_ptr<snailfish_number> added_number = std::make_shared<snailfish_number>();

    added_number->left = std::move(left);
    added_number->left->parent = added_number;
    added_number->left->side = LEFT;
    added_number->right = std::move(right);
    added_number->right->parent = added_number;
    added_number->right->side = RIGHT;

#ifdef DEBUG
    std::cout << "After addition: ";
    print_snailfish(added_number);
    std::cout << '\n';
#endif

    return added_number;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    snailfish_ptr sum = snailfish_add(input[0], input[1]);
    snailfish_reduce(sum);
#ifdef DEBUG
    print_snailfish(sum);
    std::cout << '\n';
#endif

    for (size_t i = 2; i < input.size(); i++) {
        sum = snailfish_add(sum, input[i]);
        snailfish_reduce(sum);
#ifdef DEBUG
        print_snailfish(sum);
        std::cout << '\n';
#endif
    }

    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

//    for (auto i : input) {
//        snailfish_explode(i);
//    }

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
