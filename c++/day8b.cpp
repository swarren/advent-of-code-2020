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
public:
    using Instruction::Instruction;

    void execute(State &state) override {
        state.acc += m_param;
        state.pc += 1;
    }
};

class CorruptableInstruction : public Instruction {
public:
    using Instruction::Instruction;

    virtual Instruction *corrupted() = 0;
};

class Nop;

class Jump : public CorruptableInstruction {
public:
    using CorruptableInstruction::CorruptableInstruction;

    void execute(State &state) override {
        state.pc += m_param;
    }

    Instruction *corrupted() override;
};

class Nop : public CorruptableInstruction {
public:
    using CorruptableInstruction::CorruptableInstruction;

    void execute(State &state) override {
        state.pc += 1;
    }

    Instruction *corrupted() override;
};

inline Instruction *Jump::corrupted() {
    return new Nop(m_param);
}
inline Instruction *Nop::corrupted() {
    return new Jump(m_param);
}

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
    using ProgramAndState = std::pair<Program, State>;
    std::vector<ProgramAndState> programAndStates;

    for (int i = 0; i < program.size(); ++i) {
        Program corruptedProgram = program;
        CorruptableInstruction *ci = dynamic_cast<CorruptableInstruction *>(program[i]);
        if (ci == nullptr)
            continue;
        corruptedProgram[i] = ci->corrupted();
        programAndStates.push_back(ProgramAndState(corruptedProgram, State()));
    }

    while (true) {
        for (ProgramAndState &pas : programAndStates) {
            const Program &program = pas.first;
            State &s = pas.second;
            if (s.pc == program.size())
                return s.acc;
            program[s.pc]->execute(s);
        }
    }
}

int main(void) {
    std::cout << answer(readParseInput("../input/day8.txt")) << '\n';
    return 0;
}
