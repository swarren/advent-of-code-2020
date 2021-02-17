#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

struct State {
    int pc;
    int acc;
};

class Instruction {
public:
    Instruction(int param) : m_param(param) {}
    virtual ~Instruction() {}
    virtual void execute(State &state) = 0;
protected:
    int m_param;
};

class Accumulate : public Instruction {
    using Instruction::Instruction;

    void execute(State &state) override {
        state.acc += m_param;
        state.pc += 1;
    }
};

class Jump : public Instruction {
    using Instruction::Instruction;

    void execute(State &state) override {
        state.pc += m_param;
    }
};


class Nop : public Instruction {
    using Instruction::Instruction;

    void execute(State &state) override {
        state.pc += 1;
    }
};

using Program = std::vector<Instruction *>;

Program readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Program program;

    std::string line;
    while (std::getline(file, line)) {
        int param = std::atoi(&line[4]);
        Instruction *instruction = 0;
        if (line.starts_with("acc")) {
            instruction = new Accumulate(param);
        } else if (line.starts_with("jmp")) {
            instruction = new Jump(param);
        } else {
            instruction = new Nop(param);
        }
        program.push_back(instruction);
    }
    file.close();

    return program;
}

int answer(const Program &program) {
    State s = {0, 0};
    std::set<int> seenPcs;

    while (true) {
        if (seenPcs.find(s.pc) != seenPcs.end())
            return s.acc;
        seenPcs.insert(s.pc);
        program[s.pc]->execute(s);
    }
}

int main(void) {
    std::cout << answer(readParseInput("../input/day8.txt")) << '\n';
    return 0;
}
