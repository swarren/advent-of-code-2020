#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using Passport = std::map<std::string, std::string>;
using Input = std::vector<Passport>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;
    Passport *mPtr;

    mPtr = &input.emplace_back();

    std::string line;
    while (std::getline(file, line)) {
        auto length = line.length();
        if (!length) {
            mPtr = &input.emplace_back();
            continue;
        }
        std::string::size_type parsePos = 0;
        while (true) {
            auto colonPos = line.find(':', parsePos);
            if (colonPos == std::string::npos)
                break;
            auto spacePos = line.find(' ', colonPos);
            if (spacePos == std::string::npos)
                spacePos = length;

            std::string fieldName = line.substr(parsePos, colonPos - parsePos);
            std::string value = line.substr(colonPos + 1, spacePos - (colonPos + 1));
            mPtr->insert({fieldName, value});

            if (spacePos == length)
                break;
            parsePos = spacePos + 1;
        }
    }
    file.close();

    return input;
}

const std::string requiredFields[] = {
    "byr",
    "iyr",
    "eyr",
    "hgt",
    "hcl",
    "ecl",
    "pid",
};

bool isValidPassport(int i, const Passport &passport) {
    for (auto requiredField : requiredFields) {
        if (passport.find(requiredField) == passport.cend())
            return false;
    }
    return true;
}

int answer(const Input &input) {
    int validPassports = 0;

    int i = 0;
    for (auto passport : input) {
        i++;
        if (isValidPassport(i, passport))
            validPassports++;
    }

    return validPassports;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day4.txt")) << '\n';
    return 0;
}
