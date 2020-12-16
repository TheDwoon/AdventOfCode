#pragma once
#include "AbstractDay.h"
#include <vector>
#include <math.h>

struct Range {
    int lower;
    int upper;

    bool contains(int i) const {
        return lower <= i && i <= upper;
    }

    void merge(const Range &other) {
        lower = std::min(lower, other.lower);
        upper = std::max(upper, other.upper);
    }
};

struct Field {
    std::string name;
    Range lowerRange;
    Range upperRange;

    bool contains(int i) const {
        return lowerRange.contains(i) || upperRange.contains(i);
    }
};

typedef std::vector<int> Ticket;

struct TicketInformation {
    std::vector<Field> fields;
    Ticket myTicket;
    std::vector<Ticket> nearbyTickets;
};

class Day16 : public virtual AbstractDay<TicketInformation>{
public:
    Day16() : AbstractDay("Day 16", "input/input16.txt") {}

    TicketInformation parseInput(std::string &input) override;
    std::string runPart1(TicketInformation &input) override;
    std::string runPart2(TicketInformation &input) override;
};
