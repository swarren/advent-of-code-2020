// FIXME: This code leaks memory like a sieve

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool debug = false;

using Input = std::vector<std::string>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    std::string line;
    while (std::getline(file, line))
        input.push_back(line);
    file.close();

    return input;
}

struct Token {
    virtual std::ostream& printTo(std::ostream& os) const {
        os << "Token";
        return os;
    }
};
std::ostream& operator<<(std::ostream& os, const Token &token) {
    return token.printTo(os);
}

struct TokNum : public Token {
    TokNum(long val) : m_val(val) {}

    virtual std::ostream& printTo(std::ostream& os) const override {
        os << "TokNum(";
        os << m_val;
        os << ')';
        return os;
    }

    long m_val;
};

struct TokOperator : public Token {
    virtual TokNum *operator()(const TokNum *lhs, const TokNum *rhs) const = 0;
};

struct TokAdd : public TokOperator {
    virtual TokNum *operator()(const TokNum *lhs, const TokNum *rhs) const override {
        return new TokNum(lhs->m_val + rhs->m_val);
    }

    virtual std::ostream& printTo(std::ostream& os) const override {
        os << "TokAdd";
        return os;
    }
};

struct TokMultiply : public TokOperator {
    virtual TokNum *operator()(const TokNum *lhs, const TokNum *rhs) const override {
        return new TokNum(lhs->m_val * rhs->m_val);
    }

    virtual std::ostream& printTo(std::ostream& os) const override {
        os << "TokMultiply";
        return os;
    }
};


struct TokLParen : public Token {
    virtual std::ostream& printTo(std::ostream& os) const override {
        os << "TokLParen";
        return os;
    }
};

struct TokRParen : public Token {
    virtual std::ostream& printTo(std::ostream& os) const override {
        os << "TokRParen";
        return os;
    }
};

struct TokEof : public Token {
    virtual std::ostream& printTo(std::ostream& os) const override {
        os << "TokEof";
        return os;
    }
};

std::istream& operator>>(std::istream &is, Token *&val) {
    char c;
    do {
        is >> c;
        if (is.eof()) {
            val = new TokEof();
            return is;
        }
    } while (c == ' ');
    if (c == '(')
        val = new TokLParen();
    else if (c == ')')
        val = new TokRParen();
    else if (c == '+')
        val = new TokAdd();
    else if (c == '*')
        val = new TokMultiply();
    else
        val = new TokNum(c - '0');
    return is;
}

using Stack = std::vector<Token *>;

std::ostream& operator<<(std::ostream& os, const Stack &stack) {
    bool first = true;
    for (auto t : stack) {
        if (first)
            first = false;
        else
            os << ", ";
        os << *t;
    }
    return os;
}

long eval(const std::string &expression) {
    if (debug) std::cout << "Expression: " << expression << '\n';

    std::istringstream iss(expression);
    Stack stack;

    Token *t;
    while (true) {
        iss >> t;
        if (debug) std::cout << "Parsed token: " << *t << '\n';
        TokEof *te = dynamic_cast<TokEof *>(t);
        if (te)
            break;
        stack.push_back(t);

        while (true) {
            if (debug) std::cout << "Stack: " << stack << '\n';

            auto l = stack.size();
            if (l < 3) {
                if (debug) std::cout << "No reduction possible due to length\n";
                break;
            }

            Token *t1 = stack[l - 3];
            Token *t2 = stack[l - 2];
            Token *t3 = stack[l - 1];

            TokNum *t1n = dynamic_cast<TokNum *>(t1);
            TokNum *t2n = dynamic_cast<TokNum *>(t2);
            TokNum *t3n = dynamic_cast<TokNum *>(t3);
            TokLParen *t1lp = dynamic_cast<TokLParen *>(t1);
            TokRParen *t3rp = dynamic_cast<TokRParen *>(t3);
            TokOperator *t2op = dynamic_cast<TokOperator *>(t2);

            if (t1n && t2op && t3n) {
                if (debug) std::cout << "Reduce operator\n";
                stack[l - 3] = (*t2op)(t1n, t3n);
                stack.resize(l - 2);
            } else if (t1lp && t2n && t3rp) {
                if (debug) std::cout << "Reduce parenthesized number\n";
                stack[l - 3] = t2n;
                stack.resize(l - 2);
            } else {
                if (debug) std::cout << "No reduction known\n";
                break;
            }
        }
    }
    if (debug) std::cout << "Stack: " << stack << '\n';
    if (stack.size() != 1) {
        std::cout << "Unexpected stack length at end of evaluation\n";
        return 0;
    }

    TokNum *t1n = dynamic_cast<TokNum *>(stack[0]);
    if (!t1n) {
        std::cout << "Unexpected stack format at end of evaluation\n";
        return 0;
        }

    long result = t1n->m_val;
    if (debug) std::cout << "Evaluated to " << result << '\n';
    return result;
}

long answer(const Input &input) {
    long result = 0;
    for (const auto &expression : input)
        result += eval(expression);
    return result;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day18.txt")) << '\n';
    return 0;
}
