#include <array>
#include <iostream>
#include <vector>

using namespace std;

namespace {

constexpr array<array<int, 9>, 9> kMoveEffect = {{
    {{1, 1, 0, 1, 1, 0, 0, 0, 0}},
    {{1, 1, 1, 0, 0, 0, 0, 0, 0}},
    {{0, 1, 1, 0, 1, 1, 0, 0, 0}},
    {{1, 0, 0, 1, 0, 0, 1, 0, 0}},
    {{0, 1, 0, 1, 1, 1, 0, 1, 0}},
    {{0, 0, 1, 0, 0, 1, 0, 0, 1}},
    {{0, 0, 0, 1, 1, 0, 1, 1, 0}},
    {{0, 0, 0, 0, 0, 0, 1, 1, 1}},
    {{0, 0, 0, 0, 1, 1, 0, 1, 1}},
}};

array<int, 9> clocks_state{};
array<int, 9> move_count{};
vector<int> best_sequence;
int best_length = 1e9;

void apply_move(int move, int delta) {
    for (int i = 0; i < 9; ++i) {
        if (kMoveEffect[move][i] != 0) {
            clocks_state[i] = (clocks_state[i] + delta + 4) % 4;
        }
    }
}

vector<int> build_sequence() {
    vector<int> sequence;
    for (int move = 0; move < 9; ++move) {
        for (int repeat = 0; repeat < move_count[move]; ++repeat) {
            sequence.push_back(move + 1);
        }
    }
    return sequence;
}

void dfs(int move, int used) {
    if (used > best_length) {
        return;
    }
    if (move == 9) {
        for (int value : clocks_state) {
            if (value != 0) {
                return;
            }
        }
        vector<int> sequence = build_sequence();
        if (used < best_length || (used == best_length && sequence < best_sequence)) {
            best_length = used;
            best_sequence = sequence;
        }
        return;
    }

    const array<int, 9> saved_state = clocks_state;
    for (int repeat = 0; repeat < 4; ++repeat) {
        clocks_state = saved_state;
        for (int step = 0; step < repeat; ++step) {
            apply_move(move, 1);
        }
        move_count[move] = repeat;
        dfs(move + 1, used + repeat);
    }
    clocks_state = saved_state;
    move_count[move] = 0;
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for (int i = 0; i < 9; ++i) {
        cin >> clocks_state[i];
    }

    dfs(0, 0);

    for (int i = 0; i < static_cast<int>(best_sequence.size()); ++i) {
        if (i > 0) {
            cout << ' ';
        }
        cout << best_sequence[i];
    }
    cout << '\n';
    return 0;
}
