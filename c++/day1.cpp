#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::vector<int> result;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int num;
        iss >> num;
        result.push_back(num);
    }
    file.close();

    return result;
}

int main(void) {
    auto input = readParseInput("../input/day1.txt");

    for (auto it0 = input.begin(); it0 != input.end(); ++it0) {
        for (auto it1 = it0 + 1; it1 != input.end(); ++it1) {
            int sum = *it0 + *it1;
            if (sum == 2020) {
                std::cout << *it0 * *it1 << std::endl; 
            }
        }
    }
    return 0;
}
