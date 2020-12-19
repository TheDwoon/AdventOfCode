#include "Day19.h"

MessageCollection Day19::parseInput(std::string& input)
{
    MessageCollection collection;

    int ruleEnd = input.find("\n\n");
    std::string rules = input.substr(0, ruleEnd);

    // parse rules
    std::stringstream ruleStream(rules);
    int i;
    char c;
    while (ruleStream && ruleStream >> i) {
        Rule rule;
        rule.id = i;

        ruleStream.get(c);
        ruleStream.get(c);  // ignore " " and :
        if (ruleStream.peek() == '"') {
            ruleStream >> c >> c; // red " and literal
            rule.c = c;
            ruleStream.get(c).get(c);
        }
        else {
            rule.c = 0;
            while (c != '\n' && ruleStream.peek() != '|' && ruleStream >> i) {
                rule.first.push_back(i);
                ruleStream.get(c);
            }

            if (ruleStream.peek() == '|') {
                ruleStream >> c;
                while (c != '\n' && ruleStream >> i) {
                    rule.second.push_back(i);
                    ruleStream.get(c);
                }
            }
        }        

        collection.rules[rule.id] = rule;
    }

    // parse messages
    std::string messages = input.substr(ruleEnd + 2);
    std::stringstream messageStream(messages);
    for (std::string line; std::getline(messageStream, line);) {
        collection.messages.push_back(line);
    }

    return collection;
}

const int MAX_DEPTH = 10000;

int matchRule(const std::string &str, int ruleId, std::map<int, Rule> &rules, int pos, int depth = 0) {
    Rule& rule = rules[ruleId];

    // check if this rule does match that character
    if (rule.c == str[pos])
        return 1;
    else if (rule.c != 0)
        return 0;

    int firstMatchSize = 0;
    bool firstMatched = rule.first.size() > 0;
    for (int i = 0; firstMatched && i < rule.first.size() && pos + firstMatchSize < str.size(); i++) {
        int matched = matchRule(str, rule.first[i], rules, pos + firstMatchSize, depth + 1);
        firstMatched = matched > 0;
        firstMatchSize += matched;
    }

    int secondMatchSize = 0;
    bool secondMatched = rule.second.size() > 0;
    for (int i = 0; secondMatched && i < rule.second.size() && pos + secondMatchSize < str.size(); i++) {
        int matched = matchRule(str, rule.second[i], rules, pos + secondMatchSize, depth + 1);
        secondMatched = matched > 0;
        secondMatchSize += matched;
    }

    if (firstMatched && secondMatched)
        return std::max(firstMatchSize, secondMatchSize);
    else if (firstMatched)
        return firstMatchSize;
    else if (secondMatched)
        return secondMatchSize;
    else
        return 0;
}

std::string Day19::runPart1(day_t& input)
{
    std::stringstream output;
    
    int matched = 0;
    for (const std::string& message : input.messages) {
        //matched += matchRule(message, 0, input.rules, 0) == message.size();
    }

    output << matched;
    return output.str();
}

std::string Day19::runPart2(day_t& input)
{
    Rule r8;
    r8.id = 8;
    r8.c = 0;
    r8.first.push_back(42);
    r8.second.push_back(42);
    r8.second.push_back(8);

    Rule r11;
    r11.id = 11;
    r11.c = 0;
    r11.first.push_back(42);
    r11.first.push_back(31);
    r11.second.push_back(42);
    r11.second.push_back(11);
    r11.second.push_back(31);

    input.rules[8] = r8;
    input.rules[11] = r11;

    std::stringstream output;
    int matched = 0;
    for (const std::string& message : input.messages) {
        int match = matchRule(message, 0, input.rules, 0);
        std::cout << message << ": " << match << std::endl;
        matched += match == message.size();
    }

    output << matched;
    return output.str();
}
