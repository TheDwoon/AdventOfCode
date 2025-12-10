#include <cstdint>
#include <cstring>
#include <vector>
#include <queue>
#include <set>

#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

struct machine_state
{
    uint16_t rating;
    uint16_t state;
    std::set<uint16_t> buttonsPushed;

    friend bool operator<(const machine_state& lhs, const machine_state& rhs) { return lhs.rating > rhs.rating; }
};

struct machine
{
    uint16_t targetState {};
    std::vector<uint16_t> buttons;
    std::vector<uint16_t> joltages;
};

void rateState(const uint16_t targetBits, machine_state& state)
{
    const uint16_t stateBits = std::popcount(state.state);

    state.rating = std::max(targetBits, stateBits) - std::min(targetBits, stateBits) + state.buttonsPushed.size();
}

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    std::vector<machine> machines;

    Parser p(buffer);
    while (!p.eof()) {
        machine m;

        // Parse target state
        p.consume('[');
        while (p.peek() == '.' || p.peek() == '#')
        {
            switch (p.read())
            {
            case '.':
                m.targetState = (m.targetState << 1) | 0;
                break;

            case '#':
                m.targetState = (m.targetState << 1) | 1;
                break;
            }
        }
        p.consume(']');
        p.consume(' ');

        // Parse buttons
        while (p.peek() != '{')
        {
            p.consume('(');
            uint16_t button = 0;

            while (p.peek() != ')')
            {
                int bit;
                p.readNumber(bit);
                p.consume(',');
                button = button | (1 << bit);
            }
            p.consume(')');
            p.consume(' ');
            m.buttons.push_back(button);
        }

        // Parse joltage
        p.consume('{');
        while (p.peek() != '}')
        {
            uint16_t joltage;
            p.readNumber(joltage);
            p.consume(',');
            m.joltages.push_back(joltage);
        }

        p.consume('}');
        p.readNewLine();
        machines.push_back(m);
    }

    for (const auto& machine : machines)
    {
        const uint16_t targetBits = std::popcount(machine.targetState);

        // setup queue
        std::priority_queue<machine_state> queue;
        {
            machine_state initialState = { 0, 0, {} };
            queue.push(initialState);
        }

        while (!queue.empty())
        {
            machine_state state = queue.top();
            queue.pop();

            if (state.state == machine.targetState)
            {
                part1 += state.buttonsPushed.size();
                break;
            }

            for (const auto& button : machine.buttons)
            {
                if (state.buttonsPushed.contains(button))
                    continue;

                machine_state newState = state;
                newState.state = newState.state ^ button;;
                newState.buttonsPushed.insert(button);
                rateState(targetBits, newState);
                queue.push(newState);
            }
        }
    }

    printf("%d\n",part1);
    printf("%d\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
