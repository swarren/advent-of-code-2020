#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Rule {
    size_t pos1;
    size_t pos2;
    char c;
    std::string pw;

    bool valid() {
        bool pos1eq = this->pw[this->pos1 - 1] == this->c;
        bool pos2eq = this->pw[this->pos2 - 1] == this->c;
        return pos1eq ^ pos2eq;
    }
};

std::vector<Rule> readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::vector<Rule> result;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Rule r;
        char c;
        // 9-12 q: qqqxhnhdmqqqqjz
        iss >> r.pos1 >> c >> r.pos2 >> r.c >> c >> r.pw;
        result.push_back(r);
    }
    file.close();

    return result;
}

int main(void) {
    auto input = readParseInput("../input/day2.txt");

    int countValid = 0;
    auto f = [&countValid](Rule &r) { if (r.valid()) countValid++; };
    for (auto r : input)
        f(r);
    std::cout << countValid << std::endl; 

    return 0;
}
