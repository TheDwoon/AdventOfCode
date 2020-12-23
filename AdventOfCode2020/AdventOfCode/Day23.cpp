#include "Day23.h"

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
    cup_t min = stack[0];
    for (int i = 1; i < stack.size(); i++)
        min = std::min(min, stack[i]);

    cup_t max = stack[0];
    for (int i = 1; i < stack.size(); i++)
        max = std::max(max, stack[i]);

    for (int i = 0; i < moves; i++) {
        // pick up cups
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

        // place current bag
        stack.push_back(current);

        moveHeadTo(stack, destination);

        // pop the destination cup of the stack and move it to the back
        cup_t head = stack.front();
        stack.pop_front();
        stack.push_back(head);

        // place the picked up cups behind it.
        stack.push_back(c1);
        stack.push_back(c2);
        stack.push_back(c3);

        // spool to current
        moveHeadTo(stack, current);

        stack.pop_front();
        stack.push_back(current);

        // head of stack is now new current
    }
}

std::string Day23::runPart1(stack_t &input) {
    std::stringstream output;

    playGame(input, 100);

    moveHeadTo(input, 1);

    for (int i = 1; i < input.size(); i++)
        output << input[i];

    return output.str();
}

std::string Day23::runPart2(stack_t &input) {
    std::stringstream output;

    for (cup_t cup = 10; cup <= 1000000; cup++)
        input.push_back(cup);

    playGame(input, 10000000);
    moveHeadTo(input, 1);

    long long star1 = input[1];
    long long star2 = input[2];

    output << (star1 * star2);
    return output.str();
}
