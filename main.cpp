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

struct Solver {
    static constexpr int kInitialBest = 1000000000;

    array<int, 9> clocks_state{};
    array<int, 9> move_count{};
    vector<int> best_sequence;
    int best_length = kInitialBest;

    void apply_move(int move, int delta) {
        for (int i = 0; i < 9; ++i) {
            if (kMoveEffect[move][i] != 0) {
                clocks_state[i] = (clocks_state[i] + delta + 4) % 4;
            }
        }
    }

    vector<int> build_sequence() const {
        vector<int> sequence;
        for (int move = 0; move < 9; ++move) {
            for (int repeat = 0; repeat < move_count[move]; ++repeat) {
                sequence.push_back(move + 1);
            }
        }
        return sequence;
    }

    void dfs(int move, int used) {
        if (used >= best_length) {
            return;
        }
        if (move == 9) {
            for (int value : clocks_state) {
                if (value != 0) {
                    return;
                }
            }
            best_length = used;
            best_sequence = build_sequence();
            return;
        }

        const array<int, 9> saved_state = clocks_state;
        for (int repeat = 3; repeat >= 0; --repeat) {
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

    vector<int> solve(const array<int, 9>& initial_state) {
        clocks_state = initial_state;
        move_count.fill(0);
        best_sequence.clear();
        best_length = kInitialBest;
        dfs(0, 0);
        return best_sequence;
    }
};

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    array<int, 9> initial_state{};
    for (int i = 0; i < 9; ++i) {
        cin >> initial_state[i];
    }

    const vector<int> answer = Solver().solve(initial_state);

    for (int i = 0; i < static_cast<int>(answer.size()); ++i) {
        if (i > 0) {
            cout << ' ';
        }
        cout << answer[i];
    }
    cout << '\n';
    return 0;
}
