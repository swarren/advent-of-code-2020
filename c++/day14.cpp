#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Mask {
    uint64_t mask1;
    uint64_t maskNon0;
};

struct State {
    Mask mask;
    std::map<uint64_t, uint64_t> mem;
};

class Op {
public:
    Op() {}
    virtual ~Op() {}
    virtual void execute(State &state) = 0;
};

class MaskOp : public Op {
public:
    MaskOp(const Mask &mask) :
        m_mask(mask)
        {}

    void execute(State &s) override {
        s.mask = m_mask;
    }
protected:
    Mask m_mask;
};

class MemOp : public Op {
public:
    MemOp(uint64_t addr, uint64_t value) :
        m_addr(addr),
        m_value(value)
        {}

    void execute(State &s) override {
        s.mem[m_addr] = (m_value & s.mask.maskNon0) | s.mask.mask1;
    }
protected:
    uint64_t m_addr, m_value;
};

using Input = std::vector<Op *>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    std::string line;
    while (std::getline(file, line)) {
        Op *op = 0;
        std::string::size_type rhsPos = line.find(" = ");
        rhsPos += 3;
        std::string::size_type lbracketPos = line.find('[');
        if (lbracketPos == std::string::npos) {
            Mask mask = {};
            for (int i = 0; i < 36; ++i) {
                mask.mask1 <<= 1;
                mask.maskNon0 <<= 1;
                if (line[rhsPos + i] == '1')
                    mask.mask1 |= 1;
                if (line[rhsPos + i] != '0')
                    mask.maskNon0 |= 1;
            }
            op = new MaskOp(mask);
        } else {
            uint64_t addr = std::atol(&line[lbracketPos + 1]);
            uint64_t value = std::atol(&line[rhsPos]);
            op = new MemOp(addr, value);
        }

        input.push_back(op);
    }
    file.close();

    return input;
}

uint64_t answer(const Input &input) {
    State s = {};

    for (Op *op : input)
        op->execute(s);

    uint64_t sum = 0;
    for (auto kv : s.mem)
        sum += kv.second;

    return sum;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day14.txt")) << '\n';
    return 0;
}
