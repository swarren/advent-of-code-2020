#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

const std::string inputText = "364297581";
const int numMoves = 10'000'000;
const int numCups = 1'000'000;

using Input = std::vector<int>;

Input readParseInput() {
    Input input;
    for (char c : inputText)
        input.emplace_back(c - '0');
    return input;
}

int dec(int v, int num) {
    v--;
    if (!v)
        v = num;
    return v;
}

long answer(const Input &input) {
    using NextCup = std::array<int, numCups + 1>;
    NextCup nextCup;

    Input::size_type inputLen = input.size();
    for (Input::size_type i = 0; i < inputLen - 1; i++)
        nextCup[input[i]] = input[(i + 1) % inputLen];
    nextCup[input[inputLen - 1]] = inputLen + 1;
    for (Input::size_type c = inputLen + 1; c < numCups; c++)
        nextCup[c] = c + 1;
    nextCup[numCups] = input[0];

    int curCup = input[0];
    for (int move = 1; move <= numMoves; ++move) {
        int after1 = nextCup[curCup];
        int after2 = nextCup[after1];
        int after3 = nextCup[after2];
        int after4 = nextCup[after3];
        nextCup[curCup] = after4;
        int destCup = dec(curCup, numCups);
        while ((destCup == after1) || (destCup == after2) || (destCup == after3))
                destCup = dec(destCup, numCups);
        int afterDestCup = nextCup[destCup];
        nextCup[destCup] = after1;
        nextCup[after3] = afterDestCup;
        curCup = nextCup[curCup];
    }

    int after1 = nextCup[1];
    int after2 = nextCup[after1];
    long answer = long(after1) * long(after2);

    return answer;
}

int main(void) {
    std::cout << answer(readParseInput()) << '\n';
    return 0;
}
