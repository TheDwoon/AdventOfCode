#include "Day09.h"

std::vector<uint64_t> Day09::parseInput(std::string& input)
{
	std::vector<uint64_t> parsed;
	std::stringstream stream(input);
	uint64_t c;
	while (stream && stream >> c)
		parsed.push_back(c);
	
	return parsed;
}

std::string Day09::runPart1(day_t& input)
{
	std::stringstream output;
	uint64_t* data = input.data();
	for (int i = 25; i < input.size(); i++) {
		if (!validateXMAS(data + i)) {
			output << data[i];
			break;
		}
	}
	
	return output.str();
}

std::string Day09::runPart2(day_t& input)
{
	std::stringstream output;
	uint64_t* data = input.data();
	uint64_t invalidNumber { 0 };
	for (int i = 25; i < input.size(); i++) {
		if (!validateXMAS(data + i)) {
			invalidNumber = data[i];
			break;
		}
	}
	
	int i = 0, j = 1;
	uint64_t sum = data[0] + data[1];
	
	while (sum != invalidNumber) {
		while (sum < invalidNumber) {
			sum += data[++j];
		}
		while (sum > invalidNumber) {
			sum -= data[i++];
		}
	}
	
	uint64_t min = data[i];
	uint64_t max = data[i];	
	for (int k = i; k <= j; k++) {
		min = std::min(min, data[k]);
		max = std::max(max, data[k]);
	}
	
	output << (min + max);
	
	return output.str();
}

bool Day09::validateXMAS(const uint64_t* v)
{
	// x1 + x2 = v
	for (int i = -25; i < 0; i++)
		for (int j = i + 1; j < 0; j++)
			if (v[i] + v[j] == v[0]) return true;
	
	return false; 
}
