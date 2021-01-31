#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using Passport = std::map<std::string, std::string>;
using Input = std::vector<Passport>;

auto readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input result;
    Passport *mPtr;

    mPtr = &result.emplace_back();

    std::string line;
    while (std::getline(file, line)) {
    auto length = line.length();
        if (!length) {
            mPtr = &result.emplace_back();
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

    return result;
}

using fieldValidityFunc = bool (*)(const std::string &);

bool validByr(const std::string &value) {
    if (value.length() != 4)
        return false;
    std::istringstream iss(value);
    int num;
    iss >> num;
    return (num >= 1920) && (num <= 2002);
}

bool validIyr(const std::string &value) {
    if (value.length() != 4)
        return false;
    std::istringstream iss(value);
    int num;
    iss >> num;
    return (num >= 2010) && (num <= 2020);
}

bool validEyr(const std::string &value) {
    if (value.length() != 4)
        return false;
    std::istringstream iss(value);
    int num;
    iss >> num;
    return (num >= 2020) && (num <= 2030);
}

bool validHgt(const std::string &value) {
    std::istringstream iss(value);
    int num;
    char units[3];
    iss >> num >> units;
    if (!strcmp(units, "cm"))
        return (num >= 150) && (num <= 193);
    else if (!strcmp(units, "in"))
        return (num >= 59) && (num <= 76);
    else
        return false;
}

bool validHcl(const std::string &value) {
    if (value.length() != 7)
        return false;
    if (value[0] != '#')
        return false;
    if (value.find_first_not_of("0123456789abcdef", 1) != std::string::npos)
        return false;
    return true;
}

const std::set<std::string> validEcls = {
    "amb",
    "blu",
    "brn",
    "gry",
    "grn",
    "hzl",
    "oth",
};

bool validEcl(const std::string &value) {
    return validEcls.find(value) != validEcls.cend();
}

bool validPid(const std::string &value) {
    if (value.length() != 9)
        return false;
    if (value.find_first_not_of("0123456789", 0) != std::string::npos)
        return false;
    return true;
}

const std::pair<std::string, fieldValidityFunc> fieldValidityChecks[] = {
    { "byr", validByr },
    { "iyr", validIyr },
    { "eyr", validEyr },
    { "hgt", validHgt },
    { "hcl", validHcl },
    { "ecl", validEcl },
    { "pid", validPid },
};

bool isValidPassport(const Passport &passport) {
    for (auto fieldValidityCheck : fieldValidityChecks) {
        const auto &fieldName = fieldValidityCheck.first;
        const auto &checkFunc = fieldValidityCheck.second;
        auto it = passport.find(fieldName);
        if (it == passport.cend())
            return false;
        if (!checkFunc((*it).second))
            return false;
    }
    return true;
}

int answer(const Input &input) {
    int validPassports = 0;
    for (auto passport : input) {
        if (isValidPassport(passport))
            validPassports++;
    }
    return validPassports;
}

int main(void) {
    const auto input = readParseInput("../input/day4.txt");
    const auto answer_ = answer(input);
    std::cout << answer_ << '\n';

    return 0;
}
