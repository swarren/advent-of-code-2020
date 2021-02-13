#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

bool debug = false;

const std::string inputText = "364297581";
const int numMoves = 100;
const int numCups = 9;

using Input = std::vector<int>;

Input readParseInput() {
    Input input;
    for (char c : inputText)
        input.emplace_back(c - '0');
    return input;
}

using NextCup = std::array<int, numCups + 1>;

struct dumpNextCup {
    const NextCup &nextCup;
    int curCup;
};

std::ostream &operator<<(std::ostream &os, const dumpNextCup& dnc) {
    int dumpCup = dnc.curCup;
    while (true) {
        if (dumpCup == dnc.curCup)
            std::cout << '(';
        std::cout << char(dumpCup + '0');
        if (dumpCup == dnc.curCup)
            std::cout << ')';
        dumpCup = dnc.nextCup[dumpCup];
        if (dumpCup == dnc.curCup)
            break;
        std::cout << ' ';
    }
    return os;
};

int dec(int v, int num) {
    v--;
    if (!v)
        v = num;
    return v;
}

std::string answer(const Input &input) {
    NextCup nextCup;

    Input::size_type numCups = input.size();
    for (Input::size_type i = 0; i < numCups; i++)
        nextCup[input[i]] = input[(i + 1) % numCups];

    int curCup = input[0];
    for (int move = 1; move <= numMoves; ++move) {
        if (debug) {
            std::cout << "-- move " << move << " --\n";
            std::cout << "cups: " << dumpNextCup{nextCup, curCup} << '\n';
        }

        int after1 = nextCup[curCup];
        int after2 = nextCup[after1];
        int after3 = nextCup[after2];
        if (debug)
            std::cout << "pick up: " << after1 << ", " << after2 << ", " << after3 << '\n';

        int after4 = nextCup[after3];
        nextCup[curCup] = after4;
        int destCup = dec(curCup, numCups);
        while ((destCup == after1) || (destCup == after2) || (destCup == after3))
                destCup = dec(destCup, numCups);
        if (debug) {
            std::cout << "destination: " << destCup << '\n';
            std::cout << '\n';
        }

        int afterDestCup = nextCup[destCup];
        nextCup[destCup] = after1;
        nextCup[after3] = afterDestCup;
        curCup = nextCup[curCup];
    }

    std::string answer(numCups - 1, '?');
    curCup = 1;
    for (int i = 0; i < numCups - 1; ++i) {
        curCup = nextCup[curCup];
        answer[i] = curCup + '0';
    }

    return answer;
}

int main(void) {
    std::cout << answer(readParseInput()) << '\n';
    return 0;
}
