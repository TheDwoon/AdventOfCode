#include "Day18.h"
#include <regex>

std::shared_ptr<Node> parseExpression(const std::vector<std::string>& tokens, int& pos);

/* Parses an expression or a literal. Nothing else */
std::shared_ptr<Node> parseNode(const std::vector<std::string> &tokens, int& pos) {
    std::string token = tokens[pos++];
    if (token == "(") {
        std::shared_ptr<Node> expr = parseExpression(tokens, pos);
        return std::make_shared<Braces>(expr);
    }
    else {
        return std::make_shared<Literal>(std::stoi(token));
    }
}

std::shared_ptr<Node> parseExpression(const std::vector<std::string> &tokens, int& pos) {    
    std::shared_ptr<Node> left = parseNode(tokens, pos);
    
    while (pos < tokens.size()) {
        std::string token = tokens[pos++];
        if (token == "+") {
            std::shared_ptr<Node> right = parseNode(tokens, pos);
            left = std::make_shared<Addition>(left, right);
        }
        else if (token == "*") {
            std::shared_ptr<Node> right = parseNode(tokens, pos);
            left = std::make_shared<Multiplication>(left, right);
        }
        else if (token == ")") {
            return left;
        }    
    }

    return left;
}

std::vector<std::shared_ptr<Node>> Day18::parseInput(std::string& input)
{
    day_t result;

    std::stringstream stream(input);
    std::regex regex("(\\d+|[+*()])");    
   
    for (std::string line; std::getline(stream, line);) {
        // tokenize single line
        std::vector<std::string> tokens;
        for (auto it = std::sregex_iterator(line.begin(), line.end(), regex); it != std::sregex_iterator(); ++it) {
            std::smatch match = *it;
            tokens.push_back(match[0].str());
        }

        // parse nodes from token
        int pos = 0;
        std::shared_ptr<Node> expr = parseExpression(tokens, pos);
        result.push_back(expr);
    }
    
    return result;
}

std::string Day18::runPart1(day_t& input)
{
    long long sum = 0;
    for (auto& expr : input) {
        long long res = expr->evaluate();
        //expr->print(std::cout);
        //std::cout << " = " << res << std::endl;
        sum += res;
    }

    std::ostringstream output;
    output << sum;
    return output.str();
}

std::shared_ptr<Node> checkNode(std::shared_ptr<Node> node) {
    switch (node->type()) {
    case BRACES: {
        std::shared_ptr<Braces> braces = std::static_pointer_cast<Braces>(node);
        braces->m_node = checkNode(braces->m_node);
        return node;
    }
    case MULTIPLICATION: {
        std::shared_ptr<Multiplication> mult = std::static_pointer_cast<Multiplication>(node);
        mult->m_left = checkNode(mult->m_left);
        mult->m_right = checkNode(mult->m_right);
        return node;
    }
    case ADDITION: {
        std::shared_ptr<Addition> addition = std::static_pointer_cast<Addition>(node);
        addition->m_left = checkNode(addition->m_left);
        addition->m_right = checkNode(addition->m_right);
        if (addition->needsReordering()) {
            std::shared_ptr<Multiplication> mult = std::static_pointer_cast<Multiplication>(addition->m_left);
            std::shared_ptr<Node> ll = mult->m_left;
            std::shared_ptr<Node> lr = mult->m_right;
            std::shared_ptr<Node> r = addition->m_right;

            std::shared_ptr<Addition> nAdd = std::make_shared<Addition>(lr, r);
            std::shared_ptr<Multiplication> nMul = std::make_shared<Multiplication>(ll, nAdd);
            return nMul;
        }
        else {
            return addition;
        }
    }
    case LITERAL:
        return node;
    }
}

std::string Day18::runPart2(day_t& input)
{
    long long sum = 0;
    for (auto expr : input) {        
        expr = checkNode(expr);
        sum += expr->evaluate();
    }

    std::stringstream output;
    output << sum;
    return output.str();
}
