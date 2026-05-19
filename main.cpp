#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> clocks(9);
    for (int i = 0; i < 9; ++i) {
        if (!(cin >> clocks[i])) {
            return 0;
        }
    }

    const int effect[9][9] = {
        {1, 1, 0, 1, 1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 1, 1, 0, 0, 0},
        {1, 0, 0, 1, 0, 0, 1, 0, 0},
        {0, 1, 0, 1, 1, 1, 0, 1, 0},
        {0, 0, 1, 0, 0, 1, 0, 0, 1},
        {0, 0, 0, 1, 1, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 1},
        {0, 0, 0, 0, 1, 1, 0, 1, 1}
    };

    int best_steps = 1e9;
    vector<int> best_sequence;

    for (int mask = 0; mask < (1 << 18); ++mask) {
        int temp = mask;
        int moves[9];
        int total = 0;
        for (int i = 0; i < 9; ++i) {
            moves[i] = temp & 3;
            temp >>= 2;
            total += moves[i];
        }

        if (total > best_steps) {
            continue;
        }

        bool ok = true;
        for (int clock = 0; clock < 9 && ok; ++clock) {
            int state = clocks[clock];
            for (int move = 0; move < 9; ++move) {
                state = (state + moves[move] * effect[move][clock]) % 4;
            }
            ok = (state == 0);
        }

        if (!ok) {
            continue;
        }

        vector<int> sequence;
        sequence.reserve(total);
        for (int move = 0; move < 9; ++move) {
            for (int count = 0; count < moves[move]; ++count) {
                sequence.push_back(move + 1);
            }
        }

        if (total < best_steps || sequence < best_sequence) {
            best_steps = total;
            best_sequence = sequence;
        }
    }

    for (int move : best_sequence) {
        cout << move << ' ';
    }
    cout << '\n';

    return 0;
}
