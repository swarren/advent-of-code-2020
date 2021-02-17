#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
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

bool valueValidForFieldRange(const FieldRange &fieldRange, int fieldVal) {
    return (fieldRange.min <= fieldVal) && (fieldVal <= fieldRange.max);
}

bool valueValidForFieldDef(const FieldDef &fieldDef, int fieldVal) {
    for (const FieldRange &fieldRange : fieldDef.ranges) {
        if (valueValidForFieldRange(fieldRange, fieldVal))
            return true;
    }
    return false;
}

long answer(const Input &input) {
    std::vector<Ticket> validNearbyTickets;

    for (const Ticket &ticket : input.nearbyTickets) {
        bool validTicket = true;
        for (int fieldVal : ticket) {
            bool validForSomeFieldDef = false;
            for (const FieldDef &fd : input.fieldDefs) {
                validForSomeFieldDef = valueValidForFieldDef(fd, fieldVal);
                if (validForSomeFieldDef)
                    break;
            }
            if (!validForSomeFieldDef) {
                validTicket = false;
                break;
            }
        }
        if (validTicket)
            validNearbyTickets.push_back(ticket);
    }

    using FieldIndex = int;
    using FieldIndexSet = std::set<FieldIndex>;
    using Possibilities = std::map<const FieldDef *, FieldIndexSet>;

    FieldIndex numFields = input.yourTicket.size();
    Possibilities possibilities;
    for (const FieldDef &fieldDef : input.fieldDefs) {
        possibilities[&fieldDef] = FieldIndexSet();
        for (FieldIndex fieldIndex = 0; fieldIndex < numFields; ++fieldIndex) {
            bool possible = true;
            for (const Ticket &ticket : validNearbyTickets) {
                int fieldVal = ticket[fieldIndex];
                if (!valueValidForFieldDef(fieldDef, fieldVal)) {
                    possible = false;
                    break;
                }
            }
            if (possible)
                possibilities[&fieldDef].insert(fieldIndex);
        }
    }

    std::map<const FieldDef *, FieldIndex> assignments;
    while (true) {
        auto pos = std::find_if(possibilities.cbegin(), possibilities.cend(), [](const auto &pv) { return pv.second.size() == 1; });
        if (pos == possibilities.end())
            break;
        const FieldDef *fieldDefP = pos->first;
        const FieldIndexSet &fieldIndexSet = pos->second;
        const FieldIndex fieldIndex = *fieldIndexSet.cbegin();
        assignments[fieldDefP] = fieldIndex;
        possibilities.erase(pos);
        for (auto &possibility : possibilities) {
            FieldIndexSet &fieldIndexSet = possibility.second;
            fieldIndexSet.erase(fieldIndex);
        }
    }

    long result = 1;
    for (auto assignment : assignments) {
        const FieldDef *fieldDefP = assignment.first;
        const FieldIndex fieldIndex = assignment.second;
        if (fieldDefP->name.starts_with("departure"))
            result *= input.yourTicket[fieldIndex];
    }

    return result;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day16.txt")) << '\n';
    return 0;
}
