#include "Day16.h"
#include <regex>

TicketInformation Day16::parseInput(std::string &input) {
    TicketInformation parsed;
    std::regex regex(R"((.*): (\d+)-(\d+) or (\d+)-(\d+))");
    auto begin = std::sregex_iterator(input.begin(), input.end(), regex);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        std::smatch match = *it;
        Field field;
        field.name = match[1].str();
        field.lowerRange = Range { std::stoi(match[2].str()), std::stoi(match[3].str())};
        field.upperRange = Range { std::stoi(match[4].str()), std::stoi(match[5].str())};

        parsed.fields.push_back(field);
    }

    int num;
    char c;

    std::stringstream stream(input);

    int yourTicketIdx = input.find("your ticket:") + 13;
    stream.seekg(yourTicketIdx);
    while (stream && stream >> num >> c)
        parsed.myTicket.push_back(num);

    int otherTicketIdx = input.find("nearby tickets:") + 16;
    stream = std::stringstream(input);
    stream.seekg(otherTicketIdx);
    for (std::string line; std::getline(stream, line);) {
        std::stringstream lineStream(line);
        Ticket ticket;
        while (lineStream && lineStream >> num) {
            lineStream >> c;
            ticket.push_back(num);
        }

        parsed.nearbyTickets.push_back(ticket);
    }

    return parsed;
}

bool isTicketValid(const Ticket& ticket, const std::vector<Field>& fields) {
    Range lowerRange = fields[0].lowerRange;
    Range upperRange = fields[0].upperRange;
    for (const Field& field : fields) {
        lowerRange.merge(field.lowerRange);
        upperRange.merge(field.upperRange);
    }

    for (int i : ticket) {
        if (!lowerRange.contains(i) && !upperRange.contains(i))
            return false;
    }

    return true;
}

std::string Day16::runPart1(TicketInformation &input) {
    std::stringstream output;

    Range lowerRange = input.fields[0].lowerRange;
    Range upperRange = input.fields[0].upperRange;
    for (Field& field : input.fields) {
        lowerRange.merge(field.lowerRange);
        upperRange.merge(field.upperRange);
    }

    int sum = 0;
    for (Ticket& ticket : input.nearbyTickets) {
        for (int i = 0; i < ticket.size(); i++) {
            if (!lowerRange.contains(ticket[i]) && !upperRange.contains(ticket[i])) {
                sum += ticket[i];
            }
        }
    }

    output << sum;
    return output.str();
}

bool isValidField(const std::vector<Ticket>& tickets, const Field& field, int col) {
    for (const Ticket& ticket : tickets) {
        if (!field.contains(ticket[col]))
            return false;
    }

    return true;
}

std::string Day16::runPart2(TicketInformation &input) {
    std::stringstream output;

    std::vector<Ticket> validTickets;
    for (const Ticket& ticket : input.nearbyTickets)
        if (isTicketValid(ticket, input.fields))
            validTickets.push_back(ticket);


    std::cout << "Valid Tickets: " << validTickets.size() << std::endl;
    int* fieldColumn = new int[input.fields.size()];
    bool* assignedColumn = new bool[input.fields.size()];
    for (int i = 0; i < input.fields.size(); i++) {
        fieldColumn[i] = -1;
        assignedColumn[i] = false;
    }

    for (int ignore = 0; ignore < input.fields.size(); ignore++) {
        for (int fieldIdx = 0; fieldIdx < input.fields.size(); fieldIdx++) {
            const Field &field = input.fields[fieldIdx];
            if (fieldColumn[fieldIdx] != -1) {
                std::cout << "Skipping " << field.name << " already known!" << std::endl << std::endl;
                continue;
            }

            int countPossibleColumns = 0;
            int possibleColumn = -1;
            for (int column = 0; column < input.fields.size(); column++) {
                if (assignedColumn[column]) {
                    std::cout << field.name << " for column " << column << " was skipped (already assigned)" << std::endl;
                    continue;
                }

                std::cout << field.name << " for column " << column << " is ";
                if (isValidField(validTickets, field, column)) {
                    possibleColumn = column;
                    ++countPossibleColumns;
                    std::cout << "VALID" << std::endl;
                } else {
                    std::cout << "invalid" << std::endl;
                }
            }

            if (countPossibleColumns == 1 && fieldColumn[fieldIdx] == -1) {
                std::cout << "! Setting " << field.name << "(" << fieldIdx << ") to column " << possibleColumn << std::endl;
                fieldColumn[fieldIdx] = possibleColumn;
                assignedColumn[possibleColumn] = true;
            } else {
                std::cout << "> Can't decide where to put " << field.name << std::endl;
            }
            std::cout << std::endl;
        }
    }

    for (int i = 0; i < input.fields.size(); i++)
        std::cout << input.fields[i].name << " at index " << fieldColumn[i] << std::endl;

    long long solution = 1;
    for (int i = 0; i < 6; i++) {
        solution *= input.myTicket[fieldColumn[i]];
    }
    output << solution;
    delete[] assignedColumn;
    delete[] fieldColumn;
    return output.str();
}
