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

        //std::cout << i << ": ";

        ruleStream.get(c);
        ruleStream.get(c);  // ignore " " and :
        if (ruleStream.peek() == '"') {
            ruleStream >> c >> c; // red " and literal
            rule.c = c;
            //std::cout << "\"" << c << "\"" << std::endl;

            ruleStream.get(c).get(c);
        }
        else {
            rule.c = 0;
            while (c != '\n' && ruleStream.peek() != '|' && ruleStream >> i) {
                rule.first.push_back(i);
                ruleStream.get(c);
                //std::cout << i << " ";
            }

            if (ruleStream.peek() == '|') {
                //std::cout << "| ";
                ruleStream >> c;
                while (c != '\n' && ruleStream >> i) {
                    //std::cout << i << " ";
                    rule.second.push_back(i);
                    ruleStream.get(c);
                }
            }
            
            //std::cout << std::endl;
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

int matchRule(const std::string str, int ruleId, std::map<int, Rule> &rules, int pos) {
    Rule& rule = rules[ruleId];

    //std::cout << "Matching rule " << ruleId << " (" << pos << ")" << std::endl;

    // check if this rule does match that character
    if (rule.c == str[pos]) {
        //std::cout << "Match result " << ruleId << " literal match" << std::endl;
        return 1;
    }

    if (rule.c != 0) {
        //std::cout << "Match result " << ruleId << " literal missmatch" << std::endl;
        return 0;
    }

    int firstMatchSize = 0;
    bool firstMatched = rule.first.size() > 0;
    for (int i = 0; firstMatched && i < rule.first.size(); i++) {
        int matched = matchRule(str, rule.first[i], rules, pos + firstMatchSize);
        firstMatched = matched > 0;
        firstMatchSize += matched;
    }

    int secondMatchSize = 0;
    bool secondMatched = rule.second.size() > 0;
    for (int i = 0; secondMatched && i < rule.second.size(); i++) {
        int matched = matchRule(str, rule.second[i], rules, pos + secondMatchSize);
        secondMatched = matched > 0;
        secondMatchSize += matched;
    }

    if (firstMatched && secondMatched) {
        //std::cout << "Match result " << ruleId << " both matched with max " << std::max(firstMatchSize, secondMatchSize) << std::endl;
        return std::max(firstMatchSize, secondMatchSize);
    }
    else if (firstMatched) {
        //std::cout << "Match result " << ruleId << " first matched " << firstMatchSize << std::endl;
        return firstMatchSize;
    }
    else if (secondMatched) {
        //std::cout << "Match result " << ruleId << " second matched " << secondMatchSize << std::endl;
        return secondMatchSize;
    }
    else {
        //std::cout << "Match result " << ruleId << " none matched (" << firstMatchSize << ", " << secondMatchSize << ")" << std::endl;
        return 0;
    }
}

std::string Day19::runPart1(day_t& input)
{
    std::stringstream output;
    
    int matched = 0;
    for (const std::string& message : input.messages) {
        matched += matchRule(message, 0, input.rules, 0) == message.size();
    }

    output << matched;
    return output.str();
}
