#include <cstring>
#include <iostream>
#include <vector>

#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

struct processor {
    int program_counter{0};
    int register_a;
    int register_b;
    int register_c;

    std::vector<unsigned char> memory;
    std::vector<unsigned char> output;
};

processor parseProcessor(const char* const buffer) {
    processor proc;

    Parser p(buffer);
    p.consume("Register A: ");
    p.readNumber(proc.register_a);
    p.readNewLine();

    p.consume("Register B: ");
    p.readNumber(proc.register_b);
    p.readNewLine();

    p.consume("Register C: ");
    p.readNumber(proc.register_c);
    p.readNewLine();
    p.readNewLine();

    char memory_value;
    proc.memory.reserve(64);
    p.consume("Program: ");
    for (int i = 0; !p.isNewLine(); i++) {
        p.readNumber(memory_value);
        proc.memory.push_back(memory_value);
        p.consume(',');
    }

    return proc;
}

int decode_combo_operand(const processor &processor, const unsigned char operand) {
    switch (operand) {
        case 0:
        case 1:
        case 2:
        case 3:
            return operand;
        case 4:
            return processor.register_a;
        case 5:
            return processor.register_b;
        case 6:
            return processor.register_c;
        case 7:
        default:
            std::exit(1);
    }
}

#ifndef NDEBUG
    void printBits(const int x) {
        printf("0b");
        for (int i = sizeof(int) * 8 - 1; i >= 0; i--) {
            if ((x & (1 << i)) != 0) {
                printf("1");
            } else {
                printf("0");
            }
        }
    }

    void printProcessorDebug(const processor &p, const unsigned char op_code, const unsigned char literal_operand) {
        printf("%d: %d [%d] (a: %10d, b: %10d, c: %10d)\n", p.program_counter, op_code, literal_operand, p.register_a, p.register_b, p.register_c);
        printf("a: "); printBits(p.register_a); printf("\n");
        printf("b: "); printBits(p.register_b); printf("\n");
        printf("c: "); printBits(p.register_c); printf("\n");
    }
#endif

void run_program(processor &p) {
    while (p.program_counter < p.memory.size()) {
        const unsigned char op_code = p.memory[p.program_counter];
        const unsigned char literal_operand = p.memory[p.program_counter + 1];

#ifndef NDEBUG
        printProcessorDebug(p, op_code, literal_operand);
#endif

        switch (op_code) {
            case 0: // adv
                p.register_a = p.register_a / (1 << decode_combo_operand(p, literal_operand));
                p.program_counter += 2;
                break;
            case 1: // bxl
                p.register_b = p.register_b ^ literal_operand;
                p.program_counter += 2;
                break;
            case 2: // bst
                p.register_b = decode_combo_operand(p, literal_operand) % 8;
                p.program_counter += 2;
                break;
            case 3: // jnz
                if (p.register_a == 0) {
                    p.program_counter += 2;
                } else {
                    p.program_counter = literal_operand;
                }
                break;
            case 4: // bxc
                p.register_b = p.register_b ^ p.register_c;
                p.program_counter += 2;
                break;
            case 5: // out
                p.output.push_back(decode_combo_operand(p, literal_operand) % 8);
                p.program_counter += 2;
                break;
            case 6: // bdv
                p.register_b = p.register_a / (1 << decode_combo_operand(p, literal_operand));
                p.program_counter += 2;
                break;
            case 7: // cdv
                p.register_c = p.register_a / (1 << decode_combo_operand(p, literal_operand));
                p.program_counter += 2;
                break;
            default:
                std::exit(1);
        }
    }
}

void runDay(const char* const buffer, const int length) {
    const processor original_processor = parseProcessor(buffer);
    processor corrupted_run = original_processor;

    // Part 1
    run_program(corrupted_run);

    printf("Part 1: ");
    for (int i = 0; i < corrupted_run.output.size(); i++) {
        if (i > 0)
            printf(",");
        printf("%d", corrupted_run.output[i]);
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
