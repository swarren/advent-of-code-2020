#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Input = std::vector<int>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    do {
        int num;
        file >> num;
        input.push_back(num);
    } while (!file.eof());
    file.close();

    return input;
}

int answer(const Input &input) {
    for (auto it0 = input.begin(); it0 != input.end(); ++it0) {
        for (auto it1 = it0 + 1; it1 != input.end(); ++it1) {
            for (auto it2 = it1 + 1; it2 != input.end(); ++it2) {
                int sum = *it0 + *it1 + *it2;
                if (sum == 2020) {
                    return *it0 * *it1 * *it2;
                }
            }
        }
    }
    return -1;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day1.txt")) << '\n';
    return 0;
}
