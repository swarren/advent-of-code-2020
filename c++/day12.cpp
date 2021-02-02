#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static const std::vector<std::pair<int, int>> headings = {
    { 0,  1}, // e
    {-1,  0}, // s
    { 0, -1}, // w
    { 1,  0}, // n
};

struct State {
    int y, x;
    int headingIndex;
};

class Instruction {
public:
    Instruction(int param) : m_param(param) {}
    virtual ~Instruction() {}
    virtual void execute(State &state) = 0;
protected:
    int m_param;
};

template<int headingIndex> class Move : public Instruction {
public:
    using Instruction::Instruction;
    void execute(State &s) override {
        s.y += m_param * headings[headingIndex].first;
        s.x += m_param * headings[headingIndex].second;
    }
};
using E = Move<0>;
using S = Move<1>;
using W = Move<2>;
using N = Move<3>;

template<int rotateDir> class Rotate : public Instruction {
public:
    using Instruction::Instruction;
    void execute(State &s) override {
        s.headingIndex += rotateDir * (m_param / 90);
        s.headingIndex %= headings.size();
    }
};
using L = Rotate<-1>;
using R = Rotate<1>;

class F : public Instruction {
public:
    using Instruction::Instruction;
    void execute(State &s) override {
        s.y += m_param * headings[s.headingIndex].first;
        s.x += m_param * headings[s.headingIndex].second;
    }
};

using Program = std::vector<Instruction *>;

auto readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Program program;

    std::string line;
    while (std::getline(file, line)) {
        int param = std::atoi(&line[1]);
        Instruction *instruction = 0;
        if (line[0] == 'E') {
            instruction = new E(param);
        } else if (line[0] == 'S') {
            instruction = new S(param);
        } else if (line[0] == 'W') {
            instruction = new W(param);
        } else if (line[0] == 'N') {
            instruction = new N(param);
        } else if (line[0] == 'L') {
            instruction = new L(param);
        } else if (line[0] == 'R') {
            instruction = new R(param);
        } else if (line[0] == 'F') {
            instruction = new F(param);
        }
        program.push_back(instruction);
    }
    file.close();

    return program;
}

int answer(const Program &program) {
    State s = {};
    for (auto i : program)
        i->execute(s);
    return std::abs(s.y) + std::abs(s.x);
}

int main(void) {
    std::cout << answer(readParseInput("../input/day12.txt")) << '\n';
    return 0;
}
