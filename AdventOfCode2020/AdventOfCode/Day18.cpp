#include "Day18.h"
#include <regex>

std::shared_ptr<Node> parseExpression(const std::vector<std::string>& tokens, int& pos);

/* Parses an expression or a literal. Nothing else */
std::shared_ptr<Node> parseNode(const std::vector<std::string> &tokens, int& pos) {
    std::string token = tokens[pos++];
    if (token == "(") {
        return parseExpression(tokens, pos);
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
        for (auto it = std::sregex_iterator(input.begin(), input.end(), regex); it != std::sregex_iterator(); ++it) {
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
    std::cout << input.size();

    long long sum = 0;
    for (auto& expr : input) {
        sum += expr->evaluate();
    }

    std::ostringstream output;
    output << sum;
    return output.str();
}
