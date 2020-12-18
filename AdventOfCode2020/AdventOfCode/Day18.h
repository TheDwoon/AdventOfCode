#pragma once
#include "AbstractDay.h"
#include <vector>
#include <iostream>

const char LITERAL = 1;
const char BRACES = 2;
const char ADDITION = 3;
const char MULTIPLICATION = 4;

class Node {
public:
	virtual long long evaluate() const = 0;
	virtual char type() const = 0;
	virtual bool needsReordering() const { return false; }
	//virtual void print(std::ostream& os) const = 0;
};

class Literal : public Node {
public:
	int m_value;
	Literal(int value) : m_value(value) {}
	long long evaluate() const override {
		return m_value;
	}
	char type() const override { return LITERAL; }

};

class Braces : public Node {
public:
	std::shared_ptr<Node> m_node;
	Braces(std::shared_ptr<Node> node) : m_node(node) {}
	long long evaluate() const override {
		return m_node->evaluate();
	}
	char type() const override { return BRACES; }
};

class Addition : public Node {
public:
	std::shared_ptr<Node> m_left;
	std::shared_ptr<Node> m_right;
	Addition(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : m_left(left), m_right(right) {}

	long long evaluate() const override {
		return m_left->evaluate() + m_right->evaluate();
	}
	char type() const override { return ADDITION; }
	bool needsReordering() const override {
		return m_left->type() == MULTIPLICATION;
	}
};

class Multiplication : public Node {
public:
	std::shared_ptr<Node> m_left;
	std::shared_ptr<Node> m_right;
	Multiplication(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : m_left(left), m_right(right) {}

	long long evaluate() const override {
		return m_left->evaluate() * m_right->evaluate();
	}
	char type() const override { return MULTIPLICATION; }
};

class Day18 : public virtual AbstractDay<std::vector<std::shared_ptr<Node>>>
{
public:
	Day18() : AbstractDay("Day 18", "input/input18.txt") {}
	std::vector<std::shared_ptr<Node>> parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
	std::string runPart2(day_t& input) override;
};

