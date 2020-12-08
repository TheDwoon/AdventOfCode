#include "Day08.h"
#include <regex>

Processor Day08::parseInput(std::string& input)
{
    Processor processor;
    
    std::regex regex("(acc|jmp|nop) ([+-]\\d+)");
    for (auto it = std::sregex_iterator(input.begin(), input.end(), regex); it != std::sregex_iterator(); ++it)
    {
        Instruction instruction;
        std::smatch match = *it;

        std::string instr = match[1].str();        
        if (instr == "jmp")
            instruction.code = op_code::JUMP;
        else if (instr == "acc")
            instruction.code = op_code::ACC;
        else if (instr == "nop")
            instruction.code = op_code::NOP;

        instruction.argument = std::stoi(match[2].str());

        processor.program.push_back(instruction);
    }

    Instruction hltInstruction;
    hltInstruction.code = op_code::HALT;
    processor.program.push_back(hltInstruction);
    return processor;
}

std::string Day08::runPart1(day_t& input)
{
    std::stringstream output;

    op_code op = op_code::HALT;
    do {
        Instruction& instr = input.program[input.ic];
        op = instr.code;
        switch (op)
        {
        case op_code::HALT:
            output << input.acc;
            break;
        case op_code::ACC:
            input.acc += instr.argument;            
        case op_code::NOP:
            input.ic += 1;
            break;
        case op_code::JUMP:
            input.ic += instr.argument;
            break;
        default:
            output << "KERNEL PANIK " << (int) instr.code << " " << instr.argument;
            op = op_code::HALT;
            break;
        }
        instr.code = op_code::HALT;
    } while (op != op_code::HALT);

    return output.str();
}

std::string Day08::runPart2(day_t& input)
{
    std::stringstream output;
    std::vector<Instruction> programClone(input.program);
    
    Processor fork;

    // RUN CODE
    op_code op = op_code::HALT;
    do {
        Instruction& instr = input.program[input.ic];
        op = instr.code;
        switch (op)
        {
        case op_code::HALT:
            // program is done
            output << input.acc;
            break;
        case op_code::ACC:
            // INCREMENT ACC
            input.acc += instr.argument;
            input.ic += 1;
            break;
        case op_code::NOP: {
            // FORK
            fork.program = std::vector<Instruction>(programClone);
            fork.acc = input.acc;
            fork.ic = input.ic;

            Instruction& fuzzyInstr = fork.program[fork.ic];
            fuzzyInstr.code = op_code::JUMP;
            op_code exitCode = runForkedProcessor(fork);
            if (exitCode == op_code::HALT) {
                op = op_code::HALT;
                output << fork.acc;
                break;
            }
            // END FORK

            input.ic += 1;
            break;
        }
        case op_code::JUMP: {
            // FORK
            fork.program = std::vector<Instruction>(programClone);
            fork.acc = input.acc;
            fork.ic = input.ic;

            Instruction& fuzzyInstr = fork.program[fork.ic];
            fuzzyInstr.code = op_code::NOP;
            op_code exitCode = runForkedProcessor(fork);
            if (exitCode == op_code::HALT) {
                op = op_code::HALT;
                output << fork.acc;
                break;
            }
            // END FORK

            input.ic += instr.argument;
            break;
        }
        case op_code::ABORT:
        default:
            output << "KERNEL PANIK " << (int)instr.code << " " << instr.argument;
            op = op_code::HALT;
            break;
        }
        instr.code = op_code::ABORT;
    } while (op != op_code::HALT && op != op_code::ABORT);

    return output.str();
}

op_code Day08::runForkedProcessor(Processor& processor)
{
    op_code op = op_code::HALT;
    do {
        Instruction& instr = processor.program[processor.ic];
        op = instr.code;
        switch (op)
        {
        case op_code::ABORT:
        case op_code::HALT:
            // program is done            
            break;
        case op_code::ACC:
            processor.acc += instr.argument;
            processor.ic += 1;
            break;
        case op_code::NOP:
            processor.ic += 1;
            break;
        case op_code::JUMP:
            processor.ic += instr.argument;
            break;
        default:
            op = op_code::ABORT;
            break;
        }
        instr.code = op_code::ABORT;
    } while (op != op_code::HALT && op != op_code::ABORT);

    return op;
}
