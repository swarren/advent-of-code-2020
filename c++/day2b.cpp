#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct Rule {
    int pos1;
    int pos2;
    char c;
    std::string pw;

    bool valid() const {
        bool pos1eq = this->pw[this->pos1 - 1] == this->c;
        bool pos2eq = this->pw[this->pos2 - 1] == this->c;
        return pos1eq ^ pos2eq;
    }
};

using Input = std::vector<Rule>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    // 9-12 q: qqqxhnhdmqqqqjz
    std::regex lineRegex(R"((\d+)-(\d+) (.): (.*)$)");

    std::string line;
    while (std::getline(file, line)) {
        std::smatch match;
        if (!std::regex_match(line, match, lineRegex))
            continue;
        input.emplace_back(Rule{
            atoi(match.str(1).c_str()),
            atoi(match.str(2).c_str()),
            match.str(3)[0],
            match[4],
        });
    }
    file.close();

    return input;
}

int answer(const Input &input) {
    return std::ranges::count_if(input, std::bind(&Rule::valid, std::placeholders::_1));
}

int main(void) {
    std::cout << answer(readParseInput("../input/day2.txt")) << '\n';
    return 0;
}
