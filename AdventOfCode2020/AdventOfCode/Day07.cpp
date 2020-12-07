#include "Day07.h"
#include <regex>

std::map<std::string, Bag> Day07::parseInput(std::string& input)
{
	std::map<std::string, Bag> parsed;

	std::regex regex("(\\d+|^) ?([a-z]+ [a-z]+)");
	const auto end = std::sregex_iterator();

	std::string lastBag;
	for (auto it = std::sregex_iterator(input.begin(), input.end(), regex); it != end; ++it)
	{
		std::smatch match = *it;
	
		if (match[1].str().empty()) 
		{
			lastBag = match[2].str();
			parsed[lastBag].color = lastBag;
		}
		else
		{
			ContainedBag cb;
			cb.count = std::stoi(match[1].str());
			cb.color = match[2].str();
			parsed[lastBag].bags.push_back(cb);
		}
	}

	return parsed;
}

std::string Day07::runPart1(day_t& input)
{
	const std::string goldBag("shiny gold");
	int sum = 0;
	std::stringstream output;
	for (auto it = input.begin(); it != input.end(); ++it)
		sum += canContainBagWithColor(it->second, goldBag, input);

	output << sum;
	return output.str();
}

std::string Day07::runPart2(day_t& input)
{
	const std::string goldBag("shiny gold");
	std::stringstream output;
	output << input[goldBag].bags.size() << '\n';
	output << countBagsInside(input[goldBag], input);
	return output.str();
}

bool Day07::canContainBagWithColor(Bag& current, const std::string& searched_color, day_t& input)
{
	for (ContainedBag& cb : current.bags)
		if (searched_color == cb.color)
			return true;

	bool contained = false;
	for (ContainedBag& cb : current.bags)
		contained |= canContainBagWithColor(input[cb.color], searched_color, input);
		
	return contained;
}

size_t Day07::countBagsInside(Bag& current, day_t& input)
{
	size_t count = 0;
	for (ContainedBag& cb : current.bags)
		count += cb.count * (countBagsInside(input[cb.color], input) + 1);

	return count;
}
