#pragma once
#include <string>

template<typename T>
class AbstractDay
{
private:
	std::string m_inputFileName;

public:
	AbstractDay(const char* inputFileName);
	typedef T day_t;

	void runDay();
	virtual std::string getInput();

	virtual T parseInput(std::string& input) = 0;
	virtual std::string runPart1(T& input) { return "not implemented"; };
	virtual std::string runPart2(T& input) { return "not implemented"; };
};
