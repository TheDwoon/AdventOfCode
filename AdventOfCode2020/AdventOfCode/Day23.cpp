#include "Day23.h"
#include <algorithm>
#include <map>
#include <vector>
#include <assert.h>

stack_t Day23::parseInput(std::string &input) {
    stack_t stack;
    std::stringstream stream(input);
    cup_t cup;
    while (stream && stream >> cup)
        stack.push_back(cup);

    return stack;
}

void moveHeadTo(stack_t& stack, cup_t newHead) {
    while (stack.front() != newHead) {
        cup_t head = stack.front();
        stack.pop_front();
        stack.push_back(head);
    }
}

void playGame(stack_t& stack, int moves) {
    cup_t min = stack.front();
    cup_t max = stack.front();

    for (cup_t cup : stack) {
        max = std::max(max, cup);
        min = std::min(min, cup);
    }

    for (int i = 1; i <= moves; i++) {
        // pick up cups
        if (i % 1000 == 0)
            std::cout << (i / (float) moves) << std::endl;

        cup_t current = stack.front(); stack.pop_front();
        cup_t c1 = stack.front(); stack.pop_front();
        cup_t c2 = stack.front(); stack.pop_front();
        cup_t c3 = stack.front(); stack.pop_front();

        // pick destination cup
        cup_t destination = current - 1;
        while (c1 == destination || c2 == destination || c3 == destination || destination < min) {
            if (destination < min) {
                destination = max;
            } else {
                destination--;
            }
        }

        // place current at the end of the list
        stack.push_back(current);

        auto destIt = ++std::find(stack.begin(), stack.end(), destination);

        // place the picked up cups behind it.
        stack.insert(destIt, c1);
        stack.insert(destIt, c2),
        stack.insert(destIt, c3);

        // head of stack is now new current
    }
}

std::string Day23::runPart1(stack_t &input) {
    std::stringstream output;

    playGame(input, 100);

    moveHeadTo(input, 1);

    for (auto it = ++input.begin(); it !=  input.end(); ++it)
        output << *it;

    return output.str();
}

struct Node {
    Node *prev;
    Node *next;
    Node *previousValue;
    cup_t cup;

    void insertAfter(Node *node) {
        // remove node from list
        Node *nodePrev = node->prev;
        Node *nodeNext = node->next;

        nodePrev->next = nodeNext;
        nodeNext->prev = nodePrev;

        // link next with node
        next->prev = node;
        node->next = next;

        // link my next with node
        node->prev = this;
        next = node;
    }
};

std::map<cup_t, Node*> buildFastStructure(std::vector<cup_t>& stack) {
    std::map<cup_t, Node*> cups;

    cup_t min = stack.front();
    cup_t max = stack.front();
    for (const cup_t &cup : stack) {
        min = std::min(min, cup);
        max = std::max(max, cup);
    }

    for (const cup_t &cup : stack) {
        Node *node = new Node();
        node->cup = cup;

        cups[cup] = node;
    }

    for (auto it = cups.begin(); it != cups.end(); ++it) {
        cup_t prev = it->first - 1;
        if (prev < min)
            prev = max;

        it->second->previousValue = cups[prev];
    }

    Node* head = cups[stack[0]];
    Node* last = cups[stack[stack.size() - 1]];

    head->prev = last;
    last->next = head;

    Node* prev = head;
    for (int i = 1; i < stack.size(); i++) {
        Node* node = cups[stack[i]];

        prev->next = node;
        node->prev = prev;

        prev = node;
    }

    for (auto it = cups.begin(); it != cups.end(); ++it) {
        Node* node = it->second;
        assert(node->previousValue != nullptr);
        assert(node->next != nullptr);
        assert(node->prev != nullptr);
    }

    return cups;
}

std::string Day23::runPart2(stack_t &input) {
    std::stringstream output;
    std::vector<cup_t> stack(input.begin(), input.end());

    for (cup_t cup = 10; cup <= 1000000; cup++)
        input.push_back(cup);

    // fast data structure
    std::map<cup_t, Node*> nodes = buildFastStructure(stack);

    Node* currentNode = nodes[stack[0]];
    for (int i = 1; i <= 10000000; i++) {
        // pick up cups
        Node* c1 = currentNode->next;
        Node* c2 = c1->next;
        Node* c3 = c2->next;



        // pick destination
        Node* destinationNode = currentNode->previousValue;
        while (c1 == destinationNode || c2 == destinationNode || c3 == destinationNode)
            destinationNode = destinationNode->previousValue;

        destinationNode->insertAfter(c3);
        destinationNode->insertAfter(c2);
        destinationNode->insertAfter(c1);
        // place the picked up cups behind it.

        // move current to the right
        currentNode = currentNode->next;
    }

    Node* oneNode = nodes[1];
    long long star1 = oneNode->next->cup;
    long long star2 = oneNode->next->next->cup;

    output << (star1 * star2);
    return output.str();
}
