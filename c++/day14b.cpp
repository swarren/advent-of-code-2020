#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Mask {
    uint64_t mask1;
    std::vector<std::pair<int, int>> xs;
};

struct State {
    Mask *mask;
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
        s.mask = &m_mask;
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
        for (uint64_t xval = 0; xval < (uint64_t(1) << s.mask->xs.size()); ++xval) {
            uint64_t addr = m_addr | s.mask->mask1;;
            for (auto xbit : s.mask->xs) {
                int addrBit = xbit.first;
                int valBit = xbit.second;
                addr &= ~(uint64_t(1) << addrBit);
                addr |= ((xval >> valBit) & uint64_t(1)) << addrBit;
            }
            s.mem[addr] = m_value;
        }
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
            int xNum = 0;
            for (int i = 0; i < 36; ++i) {
                mask.mask1 <<= 1;
                if (line[rhsPos + i] == '1')
                    mask.mask1 |= 1;
                if (line[rhsPos + i] == 'X')
                    mask.xs.push_back(std::make_pair(35 - i, xNum++));
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
