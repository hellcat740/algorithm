#include <iostream>
#include <queue>
#include <vector>

using namespace std;

namespace {

constexpr int kMaxSymptoms = 10;
constexpr int kMaxMedicines = 100;

struct Medicine {
    int cure_mask = 0;
    int add_mask = 0;
};

int solve(int symptom_count, const vector<Medicine>& medicines) {
    const int full_mask = (1 << symptom_count) - 1;
    vector<int> distance(1 << symptom_count, -1);
    queue<int> states;

    distance[full_mask] = 0;
    states.push(full_mask);

    while (!states.empty()) {
        const int current = states.front();
        states.pop();

        if (current == 0) {
            return distance[current];
        }

        for (const Medicine& medicine : medicines) {
            const int next = (current & ~medicine.cure_mask) | medicine.add_mask;
            if (distance[next] != -1) {
                continue;
            }
            distance[next] = distance[current] + 1;
            states.push(next);
        }
    }

    return -1;
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int m;
    cin >> n >> m;
    if (n < 0 || n > kMaxSymptoms || m < 0 || m > kMaxMedicines) {
        cout << -1 << '\n';
        return 0;
    }

    vector<Medicine> medicines(m);
    for (int i = 0; i < m; ++i) {
        for (int bit = 0; bit < n; ++bit) {
            int effect;
            cin >> effect;
            if (effect == 1) {
                medicines[i].cure_mask |= (1 << bit);
            } else if (effect == -1) {
                medicines[i].add_mask |= (1 << bit);
            }
        }
    }

    cout << solve(n, medicines) << '\n';
    return 0;
}
