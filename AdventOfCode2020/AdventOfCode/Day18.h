#pragma once
#include "AbstractDay.h"
#include <vector>
#include <iostream>

class Node {
public:
	virtual int evaluate() const = 0;
	virtual void print(std::ostream& os) const = 0;
};

class Literal : public Node {
private:
	int m_value;
public:
	Literal(int value) : m_value(value) {}
	int evaluate() const override {
		return m_value;
	}
	void print(std::ostream& os) const override {
		os << " [" << m_value << "] ";
	}
};

class Braces : public Node {
private:
	std::shared_ptr<Node> m_node;
public:
	Braces(std::shared_ptr<Node> node) : m_node(node) {}
	int evaluate() const override {
		return m_node->evaluate();
	}
	void print(std::ostream& os) const override {
		os << " (";
		m_node->print(os);
		os << ") ";
	}
};

class Addition : public Node {
private:
	std::shared_ptr<Node> m_left;
	std::shared_ptr<Node> m_right;
public:
	Addition(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : m_left(left), m_right(right) {}

	int evaluate() const override {
		return m_left->evaluate() + m_right->evaluate();
	}
	void print(std::ostream& os) const override {
		os << " [";
		m_left->print(os);
		os << " + ";
		m_right->print(os);
		os << "] ";
	}
};

class Multiplication : public Node {
private:
	std::shared_ptr<Node> m_left;
	std::shared_ptr<Node> m_right;
public:
	Multiplication(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : m_left(left), m_right(right) {}

	int evaluate() const override {
		return m_left->evaluate() * m_right->evaluate();
	}
	void print(std::ostream& os) const override {
		os << " [";
		m_left->print(os);
		os << " * ";
		m_right->print(os);
		os << "] ";
	}
};

class Day18 : public virtual AbstractDay<std::vector<std::shared_ptr<Node>>>
{
public:
	Day18() : AbstractDay("Day 18", "input/input18.txt") {}
	std::vector<std::shared_ptr<Node>> parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
};

