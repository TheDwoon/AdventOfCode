#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <chrono>

template<typename T>
class AbstractDay
{
private:
	std::string m_inputFileName;
	std::string m_title;

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
void AbstractDay<T>::runDay()
{
	std::string input = getInput();
	auto t0 = std::chrono::high_resolution_clock::now();
	T parsedInput = parseInput(input);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::string output = runPart1(parsedInput);
	auto t2 = std::chrono::high_resolution_clock::now();

	auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t0);
	auto parseDuration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
	auto runDuration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

	std::cout << std::endl;
	std::cout << "######################################" << std::endl;
	std::cout << "############### " << m_title << " ###############" << std::endl;
	std::cout << "######################################" << std::endl;
	std::cout << std::endl;
	std::cout << "**************************************" << std::endl;
	std::cout << output << std::endl;
	std::cout << "*************** Task 1 ***************" << std::endl;
	std::cout << "Parsing: " << parseDuration.count() << "ms" << std::endl;
	std::cout << "Running: " << runDuration.count() << "ms" << std::endl;
	std::cout << "Total: " << totalDuration.count() << "ms" << std::endl;
	std::cout << "**************************************" << std::endl;

	input = getInput();
	t0 = std::chrono::high_resolution_clock::now();
	parsedInput = parseInput(input);
	t1 = std::chrono::high_resolution_clock::now();
	output = runPart2(parsedInput);
	t2 = std::chrono::high_resolution_clock::now();

	totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t0);
	parseDuration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
	runDuration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

	std::cout << std::endl;
	std::cout << "**************************************" << std::endl;
	std::cout << output << std::endl;
	std::cout << "*************** Task 2 ***************" << std::endl;
	std::cout << "Parsing: " << parseDuration.count() << "ms" << std::endl;
	std::cout << "Running: " << runDuration.count() << "ms" << std::endl;
	std::cout << "Total: " << totalDuration.count() << "ms" << std::endl;
	std::cout << "**************************************" << std::endl;
}

template<typename T>
std::string AbstractDay<T>::getInput()
{
	std::fstream fileStream(this->m_inputFileName.c_str());
	std::string fileContent((std::istreambuf_iterator<char>(fileStream)), (std::istreambuf_iterator<char>()));

	return fileContent;
}