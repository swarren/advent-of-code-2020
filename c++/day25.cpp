#include <iostream>

using Input = std::pair<long, long>;
const Input input = {10441485, 1004920};

// Unfortunately, neither clang nor g++ will execute the entire function at
// compile-time as a constexpr:-(
long answer(const Input &input) {
    long n = 1;
    long iters = 0;
    while (true) {
        n *= 7;
        n %= 20201227;
        iters++;
        if (n == input.first)
            break;
    }

    n = 1;
    for (long i = 0; i < iters; ++i) {
        n *= input.second;
        n %= 20201227;
    }

    return n;
}

int main(void) {
    std::cout << answer(input) << '\n';
    return 0;
}
