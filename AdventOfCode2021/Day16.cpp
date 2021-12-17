#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <cassert>
#include <deque>

#define TITLE "Day 16"

struct bits_transmission {
    std::vector<uint8_t> data;
    uint64_t bits { 0 };
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef bits_transmission day_t;

template<typename T>
T read_bits(uint32_t start_bit, uint16_t length, const bits_transmission &input) {
    T result { 0 };
    const uint8_t* raw_data = input.data.data();

    size_t block_offset = start_bit % 8;
    size_t start_idx = start_bit / 8;
    size_t end_idx = (start_bit + length + 7) / 8;
    size_t block_size = end_idx - start_idx;
    result = result ;
    switch (block_size) {
        case 1: {
            uint8_t d = raw_data[start_idx];
            d = d << block_offset;
            d = d >> (8 - length);
            result = static_cast<T>(d);
            break;
        }
        case 2: {
            uint16_t d = (raw_data[start_idx] << 8) | raw_data[start_idx + 1];
            d = d << block_offset;
            d = d >> (16 - length);
            result = static_cast<T>(d);
            break;
        }
        case 3:
        case 4: {
            uint32_t d = (raw_data[start_idx + 0] << 24) | (raw_data[start_idx + 1] << 16)
                    | (raw_data[start_idx + 2] << 8) | (raw_data[start_idx + 3]);
            d = d << block_offset;
            d = d >> (32 - length);
            result = static_cast<T>(d);
            break;
        }
        case 5:
        case 6:
        case 7:
        case 8: {
            uint64_t d = (static_cast<uint64_t>(raw_data[start_idx + 0]) << 56) | (static_cast<uint64_t>(raw_data[start_idx + 1]) << 48)
                         | (static_cast<uint64_t>(raw_data[start_idx + 2]) << 40) | (static_cast<uint64_t>(raw_data[start_idx + 3]) << 32)
                         | (static_cast<uint64_t>(raw_data[start_idx + 4]) << 24) | (static_cast<uint64_t>(raw_data[start_idx + 5]) << 16)
                         | (static_cast<uint64_t>(raw_data[start_idx + 6]) << 8) | (static_cast<uint64_t>(raw_data[start_idx + 7]));
            d = d << block_offset;
            d = d >> (64 - length);
            result = static_cast<T>(d);
            break;
        }
    }

#ifdef DEBUG
    std::cout << "read_bits(" << start_bit << ", " << length << "): " << (uint16_t) result << "\n";
#endif
    return result;
}

struct base_packet {
    uint8_t version { 0 };
    uint8_t type { 0 };
};

struct literal_packet : public base_packet {
    uint64_t literal { 0 };
};

struct operator_packet : public base_packet {
    uint8_t length_type { 0 };
    std::vector<base_packet*> packets;
};

base_packet* read_packet(const bits_transmission& input, size_t &p);

literal_packet* read_literal(const bits_transmission& input, size_t &p, uint8_t version, uint8_t type) {
    literal_packet* packet = new literal_packet;
    packet->version = version;
    packet->type = type;
    packet->literal = 0;

    uint8_t block_count { 0 };
    uint8_t block { 0 };
    do {
        block = read_bits<uint8_t>(p, 5, input);
        p += 5;
        packet->literal = (packet->literal << 4) | (block & 0xF);
        block_count++;
    } while ((block & 0b10000) != 0);

#ifdef DEBUG
    std::cout << "read_literal: version=" << (uint16_t) version << ", type=" << (uint16_t) type << ", literal=" << packet->literal << '\n';
#endif

    assert(block_count <= 16);

    return packet;
}

operator_packet* read_operator(const bits_transmission& input, size_t &p, uint8_t version, uint8_t type) {
    operator_packet* packet = new operator_packet;
    packet->version = version;
    packet->type = type;
    packet->length_type = read_bits<uint8_t>(p, 1, input);
    p += 1;

    if (packet->length_type == 0) {
        uint16_t contained_packets_size = read_bits<uint16_t>(p, 15, input);
        p += 15;

        size_t packet_end = p + contained_packets_size;
#ifdef DEBUG
        std::cout << "read_operator: length_type=" << (uint16_t) packet->length_type << ", contained_packets_size=" << contained_packets_size << '\n';
#endif
        while (p < packet_end) {
            base_packet* contained_packet = read_packet(input, p);
            packet->packets.emplace_back(contained_packet);
        }

        assert(p == packet_end);
    } else {
        uint16_t contained_packets = read_bits<uint16_t>(p, 11, input);
        p += 11;
#ifdef DEBUG
        std::cout << "read_operator: length_type=" << (uint16_t) packet->length_type << ", contained_packets=" << contained_packets << '\n';
#endif
        while (contained_packets--) {
            base_packet* contained_packet = read_packet(input, p);
            packet->packets.emplace_back(contained_packet);
        }
    }

    return packet;
}

base_packet* read_packet(const bits_transmission& input, size_t &p) {
#ifdef DEBUG
    std::cout << "Reading packet...\n";
#endif

    uint8_t version = read_bits<uint8_t>(p, 3, input);
    uint8_t type = read_bits<uint8_t>(p + 3, 3, input);

    p += 6;
    if (type == 4)
        return read_literal(input, p, version, type);
    else
        return read_operator(input, p, version, type);
}

day_t parseInput(std::string &input) {
    day_t parsed;

    uint8_t block_count {0 };
    uint8_t block { 0 };
    for (char c : input) {
        if (c == '\n')
            continue;

        block = block << 4;
        switch (c) {
            case '0': block = block | 0b0000; break;
            case '1': block = block | 0b0001; break;
            case '2': block = block | 0b0010; break;
            case '3': block = block | 0b0011; break;
            case '4': block = block | 0b0100; break;
            case '5': block = block | 0b0101; break;
            case '6': block = block | 0b0110; break;
            case '7': block = block | 0b0111; break;
            case '8': block = block | 0b1000; break;
            case '9': block = block | 0b1001; break;
            case 'A': block = block | 0b1010; break;
            case 'B': block = block | 0b1011; break;
            case 'C': block = block | 0b1100; break;
            case 'D': block = block | 0b1101; break;
            case 'E': block = block | 0b1110; break;
            case 'F': block = block | 0b1111; break;
        }

        if (++block_count % 2 == 0) {
            parsed.data.push_back(block);
            block = 0;
        }
    }

    if (block_count % 2 != 0)
        parsed.data.push_back(block);

    parsed.bits = block_count * 4;

    return parsed;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    size_t p { 0 };
    operator_packet* packet = (operator_packet*) read_packet(input, p);

    std::deque<base_packet*> queue;
    queue.push_back(packet);

    uint32_t version_sum { 0 };
    while (!queue.empty()) {
        base_packet* p = queue.front();
        version_sum += p->version;
        if (p->type != 4) {
            for (base_packet* cp : ((operator_packet*) p)->packets) {
                queue.push_back(cp);
            }
        }

        queue.pop_front();
    }

    output << version_sum;

    return output.str();
}

uint64_t packet_value(base_packet* base) {
    if (base->type == 4) {
        literal_packet* p = (literal_packet*) base;
        return p->literal;
    }

    uint64_t value { 0 };
    operator_packet* p = (operator_packet*) base;
    switch (p->type) {
        // SUM
        case 0:
            for (base_packet* sub_packet : p->packets)
                value += packet_value(sub_packet);
            break;
        // MUL
        case 1:
            value = 1;
            for (base_packet* sub_packet : p->packets)
                value *= packet_value(sub_packet);
            break;
        // MIN
        case 2:
            value = packet_value(p->packets[0]);
            for (base_packet* sub_packet : p->packets)
                value = std::min(value, packet_value(sub_packet));
            break;
        // MAX
        case 3:
            value = packet_value(p->packets[0]);
            for (base_packet* sub_packet : p->packets)
                value = std::max(value, packet_value(sub_packet));
            break;
        // GREATER THAN
        case 5:
            assert(p->packets.size() == 2);
            if (packet_value(p->packets[0]) > packet_value(p->packets[1]))
                value = 1;
            else
                value = 0;
            break;
        // LESS THAN
        case 6:
            assert(p->packets.size() == 2);
            if (packet_value(p->packets[0]) < packet_value(p->packets[1]))
                value = 1;
            else
                value = 0;
            break;
        // EQUAL
        case 7:
            assert(p->packets.size() == 2);
            if (packet_value(p->packets[0]) == packet_value(p->packets[1]))
                value = 1;
            else
                value = 0;
            break;
    }

    return value;
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    size_t p { 0 };
    operator_packet* packet = (operator_packet*) read_packet(input, p);
    uint64_t value = packet_value(packet);
    output << value;

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
