#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Rule {
    size_t min;
    size_t max;
    char c;
    std::string pw;

    bool valid() {
        size_t count = std::count(this->pw.begin(), this->pw.end(), this->c);
        return count >= min && count <= max;
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
        iss >> r.min >> c >> r.max >> r.c >> c >> r.pw;
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
