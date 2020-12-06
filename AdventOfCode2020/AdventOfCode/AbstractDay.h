#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <chrono>
#include <sstream>

template<typename T>
class AbstractDay
{
private:
	std::string m_inputFileName;
	std::string m_title;

	std::string formatTime(std::chrono::duration<long long, std::nano> t);

public:
	AbstractDay(const char* title, const char* inputFileName);
	typedef T day_t;

	void runDay();
	std::string getInput();

	virtual T parseInput(std::string& input) = 0;
	virtual std::string runPart1(T& input) { return "not implemented"; };
	virtual std::string runPart2(T& input) { return "not implemented"; };
};

template<typename T>
AbstractDay<T>::AbstractDay(const char* title, const char* inputFileName) : m_title(title), m_inputFileName(inputFileName)
{

}

template<typename T>
std::string AbstractDay<T>::formatTime(std::chrono::duration<long long, std::nano> t) {
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

template<typename T>
void AbstractDay<T>::runDay()
{
	std::cout << "######################################" << std::endl;
	std::cout << "############### " << m_title << " ###############" << std::endl;
	std::cout << "######################################" << std::endl;
	std::cout << std::endl;

	std::string input = getInput();
	auto t0 = std::chrono::high_resolution_clock::now();
	T parsedInput = parseInput(input);
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

	input = getInput();
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

template<typename T>
std::string AbstractDay<T>::getInput()
{
	std::fstream fileStream(this->m_inputFileName.c_str());
	std::string fileContent((std::istreambuf_iterator<char>(fileStream)), (std::istreambuf_iterator<char>()));

	return fileContent;
}