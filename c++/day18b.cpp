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

    bool eof = false;
    while (!eof) {
        Token *t;
        iss >> t;
        if (debug) std::cout << "Parsed token: " << *t << '\n';
        stack.push_back(t);

        while (true) {
            if (debug) std::cout << "Stack: " << stack << '\n';

            auto l = stack.size();
            Token *t4 = (l > 3) ? stack[l - 4] : nullptr;
            Token *t3 = (l > 2) ? stack[l - 3] : nullptr;
            Token *t2 = (l > 1) ? stack[l - 2] : nullptr;
            Token *t1 = (l > 0) ? stack[l - 1] : nullptr;

            TokNum *t4n = dynamic_cast<TokNum *>(t4);
            TokNum *t3n = dynamic_cast<TokNum *>(t3);
            TokNum *t2n = dynamic_cast<TokNum *>(t2);
            TokNum *t1n = dynamic_cast<TokNum *>(t1);
            TokLParen *t3lp = dynamic_cast<TokLParen *>(t3);
            TokMultiply *t3mult = dynamic_cast<TokMultiply *>(t3);
            TokAdd *t2add = dynamic_cast<TokAdd *>(t2);
            TokRParen *t1rp = dynamic_cast<TokRParen *>(t1);
            TokEof *t1eof = dynamic_cast<TokEof *>(t1);

            if (t3lp && t2n && t1rp) {
                if (debug) std::cout << "Reduce parenthesized number\n";
                stack[l - 3] = t2n;
                stack.resize(l - 2);
            } else if (t3n && t2add && t1n) {
                if (debug) std::cout << "Reduce add\n";
                stack[l - 3] = (*t2add)(t3n, t1n);
                stack.resize(l - 2);
            } else if (t4n && t3mult && t2n && (t1rp || t1eof)) {
                if (debug) std::cout << "Reduce multiply\n";
                stack[l - 4] = (*t3mult)(t4n, t2n);
                stack[l - 3] = t1;
                stack.resize(l - 2);
            } else if (t2n && t1eof) {
                stack.resize(l - 1);
                eof = true;
                break;
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
