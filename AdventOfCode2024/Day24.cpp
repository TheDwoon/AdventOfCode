#include <algorithm>
#include <cstring>
#include <array>
#include <cstdint>
#include <map>
#include <set>
#include <stdexcept>
#include <vector>
#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

enum gate_type {
    XOR,
    AND,
    OR
};

struct gate {
    std::array<char, 3> input_a {};
    gate_type operation;
    std::array<char, 3> input_b {};
};

void parseInput(const char* const buffer, std::map<std::array<char, 3>, bool> &gate_states, std::map<std::array<char, 3>, gate> &gates) {
    Parser p(buffer);

    while (!p.isNewLine()) {
        std::array<char, 3> gate_name {};
        p.readToBuffer(gate_name.data(), 3);
        p.consume(": ");

        const bool value = p.read() == '1';
        gate_states[gate_name] = value;

        p.readNewLine();
    }

    p.readNewLine();

    while (!p.eof()) {
        gate gate;
        p.readToBuffer(gate.input_a.data(), 3);
        p.consume(' ');

        if (p.consume("OR")) {
            gate.operation = OR;
        } else if (p.consume("AND")) {
            gate.operation = AND;
        } else if (p.consume("XOR")) {
            gate.operation = XOR;
        } else {
            assert(false);
        }

        p.consume(' ');
        p.readToBuffer(gate.input_b.data(), 3);
        p.consume(" -> ");

        std::array<char, 3> output_name {};
        p.readToBuffer(output_name.data(), 3);
        p.readNewLine();

        gates[output_name] = gate;
    }
}

uint64_t get_gate_value(std::map<std::array<char, 3>, bool> &gate_states, const std::map<std::array<char, 3>, gate> &gates, const std::array<char, 3> &gate_name) {
    const auto it = gate_states.find(gate_name);
    if (it == gate_states.end()) {
        const auto gate = gates.find(gate_name);
        assert(gate != gates.end());

        const bool input_a = get_gate_value(gate_states, gates, gate->second.input_a);
        const bool input_b = get_gate_value(gate_states, gates, gate->second.input_b);

        bool value = false;
        switch (gate->second.operation) {
            case XOR:
                value = input_a ^ input_b;
                break;
            case AND:
                value = input_a && input_b;
                break;
            case OR:
                value = input_a || input_b;
                break;
        }

        gate_states[gate_name] = value;
        return value;
    } else {
        return it->second;
    }
}

bool is_input_gate(const std::array<char, 3> &gate_name) {
    return gate_name[0] == 'x' || gate_name[0] == 'y';
}

bool is_carry_gate(const gate& gate) {
    return gate.operation == OR;
}

const gate& get_gate(const std::map<std::array<char, 3>, gate> &gates, const std::array<char, 3> &gate_name) {
    const auto it = gates.find(gate_name);
    assert(it != gates.end());
    return it->second;
}

bool is_full_adder_carry(const std::map<std::array<char, 3>, gate> &gates, const std::array<char, 3> &gate_name, std::set<std::array<char, 3>> &invalid_gates) {
    const gate& carry_gate = get_gate(gates, gate_name);
    if (carry_gate.operation != OR) {
        invalid_gates.insert(gate_name);
        return false;
    }

    const gate& carry_a = get_gate(gates, carry_gate.input_a);
    const gate& carry_b = get_gate(gates, carry_gate.input_b);

    if (carry_a.operation != AND) {
        invalid_gates.insert(carry_gate.input_a);
        return false;
    }

    if (carry_b.operation != AND) {
        invalid_gates.insert(carry_gate.input_b);
        return false;
    }

    if (is_input_gate(carry_a.input_a) && is_input_gate(carry_a.input_b)) {
        const gate& stage_a = get_gate(gates, carry_b.input_a);
        const gate& stage_b = get_gate(gates, carry_b.input_b);

        if (stage_a.operation == XOR && stage_b.operation == OR) {
            return true;
        } else if (stage_a.operation == OR && stage_b.operation == XOR) {
            return true;
        } else {
            if (!(stage_a.operation == AND && is_input_gate(stage_a.input_a) && is_input_gate(stage_a.input_b))
                && !(stage_b.operation == AND && is_input_gate(stage_b.input_a) && is_input_gate(stage_b.input_b))) {
                invalid_gates.insert(carry_gate.input_b);
                return false;
            } else {
                return true;
            }
        }
    } else if (is_input_gate(carry_b.input_a) && is_input_gate(carry_b.input_b)) {
        const gate& stage_a = get_gate(gates, carry_a.input_a);
        const gate& stage_b = get_gate(gates, carry_a.input_b);

        if (stage_a.operation == XOR && stage_b.operation == OR) {
            return true;
        } else if (stage_a.operation == OR && stage_b.operation == XOR) {
            return true;
        } else {
            if (!(stage_a.operation == AND && is_input_gate(stage_a.input_a) && is_input_gate(stage_a.input_b))
                && !(stage_b.operation == AND && is_input_gate(stage_b.input_a) && is_input_gate(stage_b.input_b))) {
                // FIXME: If stage_a and stage_b are both XOR; I have no idea which one is fed
                invalid_gates.insert(carry_gate.input_a);
                return false;
            } else {
                return true;
            }
        }
    } else {
        if (is_input_gate(carry_a.input_a) || is_input_gate(carry_a.input_b)) {
            invalid_gates.insert(carry_gate.input_a);
        }
        if (is_input_gate(carry_b.input_a) || is_input_gate(carry_b.input_b)) {
            invalid_gates.insert(carry_gate.input_b);
        }

        return false;
    }
}

bool is_full_adder(const std::map<std::array<char, 3>, gate> &gates, const std::array<char, 3> &gate_name, std::set<std::array<char, 3>> &invalid_gates) {
    const gate& adder_gate = get_gate(gates, gate_name);
    if (adder_gate.operation != XOR) {
        invalid_gates.insert(gate_name);
        return false;
    }

    const gate& adder_a = get_gate(gates, adder_gate.input_a);
    const gate& adder_b = get_gate(gates, adder_gate.input_b);

    if (adder_a.operation == XOR && adder_b.operation == OR) {
        bool ret = is_full_adder_carry(gates, adder_gate.input_b, invalid_gates);
        if (!is_input_gate(adder_a.input_a) || !is_input_gate(adder_a.input_b)) {
            invalid_gates.insert(adder_gate.input_a);
            ret = false;
        }

        return ret;
    } else if (adder_a.operation == OR && adder_b.operation == XOR) {
        bool ret = is_full_adder_carry(gates, adder_gate.input_a, invalid_gates);
        if (!is_input_gate(adder_b.input_a) || !is_input_gate(adder_b.input_b)) {
            invalid_gates.insert(adder_gate.input_a);
            ret = false;
        }

        return ret;
    } else {
        if (adder_a.operation == AND) {
            invalid_gates.insert(adder_gate.input_a);
        }

        if (adder_b.operation == AND) {
            invalid_gates.insert(adder_gate.input_b);
        }

        return false;
    }
}

void runDay(const char* const buffer, const int length) {
    std::map<std::array<char, 3>, bool> gate_states;
    std::map<std::array<char, 3>, gate> gates;

    parseInput(buffer, gate_states, gates);

    uint64_t part1 = 0;
    for (int i = 0; i < 64; i++) {
        std::array<char, 3> gate_name { 'z' , static_cast<char>(i / 10 + '0'), static_cast<char>(i % 10 + '0') };
        const auto it = gates.find(gate_name);
        if (it != gates.end()) {
            part1 = part1 | (get_gate_value(gate_states, gates, it->first) << i);
        } else {
            break;
        }
    }

    int part2 = 0;
    std::set<std::array<char, 3>> invalid_gates;
    for (int i = 44; i >= 2; i--) {
        std::array<char, 3> gate_name { 'z', static_cast<char>(i / 10 + '0'), static_cast<char>(i % 10 + '0') };
        is_full_adder(gates, gate_name, invalid_gates);
    }

    // know fucked
    // z10
    // mkk
    // wjb
    // cvp

    // cvp,mkk,skc,wcb,wjb,z10,z14,z34 -> wrong

    printf("Part 1: %llu\n",part1);
    printf("Part 2: ");
    int i = 0;
    for (const auto& gate_name : invalid_gates) {
        if (i++ > 0)
            printf(",");
        printf("%c%c%c", gate_name[0], gate_name[1], gate_name[2]);
    }
    printf("\n");
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
