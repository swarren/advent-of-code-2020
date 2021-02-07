#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

bool debug = false;

struct RuleItem {
    RuleItem (bool isCh_, char ch_, int ruleNum_) :
        isCh(isCh_),
        ch(ch_),
        ruleNum(ruleNum_)
        {}
    bool isCh;
    char ch;
    int ruleNum;
};
using RuleOption = std::vector<RuleItem>;
using Rule = std::vector<RuleOption>;
using Rules = std::map<int, Rule>;
using Messages = std::vector<std::string>;

std::ostream &operator<<(std::ostream &os, const RuleItem &item) {
    if (item.isCh) {
        std::cout << '"' << item.ch << '"';
    } else {
        std::cout << item.ruleNum;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const RuleOption &option) {
    bool first = true;
    for (const RuleItem &item : option) {
        if (!first)
            std::cout << ' ';
        else
            first = false;
        std::cout << item;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Rule &rule) {
    bool first = true;
    for (const RuleOption &option : rule) {
        if (!first)
            std::cout << " | ";
        else
            first = false;
        std::cout << option;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Rules &rules) {
    for (const auto &nr : rules) {
        int ruleNum = nr.first;
        const Rule &rule = nr.second;
        std::cout << ruleNum << ": " << rule << '\n';
    }
    return os;
}

struct Input {
    Rules rules;
    Messages messages;
};

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    std::string line;
    // 8: 42
    // 19: 33 46
    // 122: 117 24 | 33 67
    // 33: "a"
    while (std::getline(file, line)) {
        if (!line.length())
            break;
        std::istringstream iss(line);
        std::string token;

        iss >> token;
        int ruleNum = atoi(token.c_str());
        Rule *r = &(input.rules.emplace(ruleNum, Rule()).first->second);
        RuleOption *ropt = &r->emplace_back();

        while (true) {
            iss >> token;
            if (token == "|") {
                ropt = &r->emplace_back();
            } else if (token[0] == '"') {
                char ch = token[1];
                ropt->emplace_back(true, ch, 0);
            } else {
                int childRuleNum = atoi(token.c_str());
                ropt->emplace_back(false, 0, childRuleNum);
            }
            if (iss.eof())
                break;
        }
    }
    while (std::getline(file, line)) {
        input.messages.push_back(line);
    }
    file.close();

    return input;
}

using Matches = std::vector<int>;

std::ostream &operator<<(std::ostream &os, const Matches &matches) {
    bool first = true;
    for (int messageIndex : matches) {
        if (!first)
            std::cout << ',';
        else
            first = false;
        std::cout << messageIndex;
    }
    return os;
}

struct Indent {
    Indent(int i_) : i(i_) {}
    int i;
};

std::ostream &operator<<(std::ostream &os, const Indent &indent) {
    os << std::string(indent.i, ' ');
    return os;
}

Matches matchesAt(const Rules &rules, const std::string &message, int MessageIndex, const Rule &rule, Indent indent);

Matches matchesAt(const Rules &rules, const std::string &message, int messageIndex, const RuleItem &item, Indent indent) {
    if (debug) std::cout << indent << "matchesAt(Item): msgIndex: " << messageIndex << " try item: " << item << '\n';
    Indent subIndent(indent.i + 2);
    Matches matches;
    if (item.isCh) {
        if (messageIndex < message.length() && message[messageIndex] == item.ch)
            matches.push_back(messageIndex + 1);
    } else {
        const Rule &subRule = rules.at(item.ruleNum);
        matches = matchesAt(rules, message, messageIndex, subRule, subIndent);
    }
    if (debug) std::cout << indent << "matchesAt(Item): matches: " << matches << '\n';
    return matches;
}

Matches matchesAt(const Rules &rules, const std::string &message, int messageIndex, const RuleOption &option, Indent indent) {
    if (debug) std::cout << indent << "matchesAt(Option): msgIndex: " << messageIndex << " try option: " << option << '\n';
    Indent subIndent(indent.i + 2);
    Matches matches{messageIndex};
    for (const RuleItem &item : option) {
        Matches nextMatches;
        for (const int index : matches) {
            Matches itemMatchesAtMatch = matchesAt(rules, message, index, item, subIndent);
            nextMatches.insert(nextMatches.end(), itemMatchesAtMatch.cbegin(), itemMatchesAtMatch.cend());
        }
        matches = nextMatches;
    }
    if (debug) std::cout << indent << "matchesAt(Option): matches: " << matches << '\n';
    return matches;
}

Matches matchesAt(const Rules &rules, const std::string &message, int messageIndex, const Rule &rule, Indent indent) {
    if (debug) std::cout << indent << "matchesAt(Rule): msgIndex: " << messageIndex << " try rule: " << rule<< '\n';
    Indent subIndent(indent.i + 2);
    Matches matches;
    for (const RuleOption &option : rule) {
        Matches optionMatches = matchesAt(rules, message, messageIndex, option, subIndent);
        matches.insert(matches.end(), optionMatches.cbegin(), optionMatches.cend());
    }
    if (debug) std::cout << indent << "matchesAt(Rule): matches: " << matches << '\n';
    return matches;
}

bool isMatch(const Rules &rules, const std::string &message) {
    const Matches matches = matchesAt(rules, message, 0, rules.at(0), 2);
    return std::find(matches.cbegin(), matches.cend(), message.length()) != matches.cend();
}

int answer(const Input &input) {
    if (debug) std::cout << input.rules;

    int result = 0;
    for (const auto &m : input.messages) {
        if (debug) std::cout << "checking " << m << '\n';
        bool match = isMatch(input.rules, m);
        if (debug) std::cout << "... match? " << match << '\n';
        if (match)
            result++;
    }
    return result;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day19.txt")) << '\n';
    return 0;
}
