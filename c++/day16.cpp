#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct FieldRange {
    int min;
    int max;
};
struct FieldDef {
    std::string name;
    std::vector<FieldRange> ranges;
};
using Ticket = std::vector<int>;
struct Input {
    std::vector<FieldDef> fieldDefs;
    Ticket yourTicket;
    std::vector<Ticket> nearbyTickets;
};

// 680,418,202,55,792,800,896,801,312,252,721,702,24,112,608,837,98,222,797,364
void parseTicket(std::string line, Ticket &ticket) {
    std::string::size_type parsePos = 0;
    while (parsePos != std::string::npos) {
        ticket.emplace_back(atoi(&line[parsePos]));
        parsePos = line.find(',', parsePos);
        if (parsePos != std::string::npos)
            parsePos++;
    }
}

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;
    std::string line;

    // list of: departure location: 43-237 or 251-961
    while (std::getline(file, line)) {
        if (!line.length())
            break;
        FieldDef *fd = &input.fieldDefs.emplace_back();

        std::string::size_type parsePos = 0;
        std::string::size_type colonPos = line.find(':', parsePos);
        fd->name = line.substr(parsePos, colonPos - parsePos);
        parsePos = colonPos + 2;

        while (parsePos != std::string::npos) {
            FieldRange *fr = &fd->ranges.emplace_back();

            fr->min = atoi(&line[parsePos]);
            parsePos = line.find('-', parsePos) + 1;

            fr->max = atoi(&line[parsePos]);
            parsePos = line.find(' ', parsePos);
            if (parsePos != std::string::npos)
                parsePos += 4; // ' or '
        }
    }

    // your ticket:
    std::getline(file, line);
    // ticket
    std::getline(file, line);
    parseTicket(line, input.yourTicket);

    // blank line
    std::getline(file, line);

    // nearby tickets:
    std::getline(file, line);
    // list of: ticket
    while (std::getline(file, line)) {
        parseTicket(line, input.nearbyTickets.emplace_back());
    }

    file.close();

    return input;
}

int answer(const Input &input) {
    int result = 0;

    for (const Ticket &t : input.nearbyTickets) {
        for (int fieldVal : t) {
            bool validForSomeFieldDef = false;
            for (const FieldDef &fd : input.fieldDefs) {
                for (const FieldRange &fr : fd.ranges) {
                    if ((fr.min <= fieldVal) && (fieldVal <= fr.max)) {
                        validForSomeFieldDef = true;
                        break;
                    }
                }
                if (validForSomeFieldDef)
                    break;
            }
            if (!validForSomeFieldDef)
                result += fieldVal;
        }
    }

    return result;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day16.txt")) << '\n';
    return 0;
}
