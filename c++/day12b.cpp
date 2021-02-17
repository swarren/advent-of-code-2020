#include <complex>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std::complex_literals;

static const std::vector<std::complex<int>> headings = {
     1,  // e
    -1i, // s
    -1,  // w
     1i, // n
};

struct State {
    std::complex<int> pos;
    std::complex<int> waypoint;
};

class Instruction {
public:
    Instruction(int param) : m_param(param) {}
    virtual ~Instruction() {}
    virtual void execute(State &state) = 0;
protected:
    int m_param;
};

template<int headingIndex> class MoveWaypoint : public Instruction {
public:
    using Instruction::Instruction;
    void execute(State &s) override {
        s.waypoint += m_param * headings[headingIndex];
    }
};
using E = MoveWaypoint<0>;
using S = MoveWaypoint<1>;
using W = MoveWaypoint<2>;
using N = MoveWaypoint<3>;

template<bool reverse> class RotateWaypoint : public Instruction {
public:
    using Instruction::Instruction;
    void execute(State &s) override {
        std::complex<int> rotation = headings[m_param / 90];
        if (reverse)
            rotation = std::conj(rotation);
        s.waypoint *= rotation;
    }
};
using L = RotateWaypoint<true>;
using R = RotateWaypoint<false>;

class F : public Instruction {
public:
    using Instruction::Instruction;
    void execute(State &s) override {
        s.pos += m_param * s.waypoint;
    }
};

using Program = std::vector<Instruction *>;

Program readParseInput(std::string fileName) {
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
    State s = {0, std::complex<int>(10, 1)};
    for (auto i : program)
        i->execute(s);
    return std::abs(s.pos.real()) + std::abs(s.pos.imag());
}

int main(void) {
    std::cout << answer(readParseInput("../input/day12.txt")) << '\n';
    return 0;
}
