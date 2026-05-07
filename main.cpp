#include <algorithm>
#include <array>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

namespace {

constexpr long long INF = numeric_limits<long long>::max() / 4;

int bit_count(int mask) {
    int count = 0;
    while (mask > 0) {
        mask &= (mask - 1);
        ++count;
    }
    return count;
}

long long build_min_number(int mask, const vector<int>& digits) {
    vector<int> values;
    for (int i = 0; i < static_cast<int>(digits.size()); ++i) {
        if (mask & (1 << i)) {
            values.push_back(digits[i]);
        }
    }

    sort(values.begin(), values.end());
    if (values.size() > 1 && values[0] == 0) {
        int first_non_zero = 1;
        while (first_non_zero < static_cast<int>(values.size()) && values[first_non_zero] == 0) {
            ++first_non_zero;
        }
        if (first_non_zero == static_cast<int>(values.size())) {
            return INF;
        }
        swap(values[0], values[first_non_zero]);
    }

    long long result = 0;
    for (int value : values) {
        result = result * 10 + value;
    }
    return result;
}

long long build_max_number(int mask, const vector<int>& digits) {
    vector<int> values;
    for (int i = 0; i < static_cast<int>(digits.size()); ++i) {
        if (mask & (1 << i)) {
            values.push_back(digits[i]);
        }
    }

    sort(values.begin(), values.end(), greater<int>());
    long long result = 0;
    for (int value : values) {
        result = result * 10 + value;
    }
    return result;
}

long long solve_odd_case(const vector<int>& digits) {
    const int n = static_cast<int>(digits.size());
    const int full_mask = (1 << n) - 1;
    const int longer_size = n / 2 + 1;
    long long answer = INF;

    for (int mask = 0; mask <= full_mask; ++mask) {
        if (bit_count(mask) != longer_size) {
            continue;
        }
        long long bigger = build_min_number(mask, digits);
        long long smaller = build_max_number(full_mask ^ mask, digits);
        answer = min(answer, bigger - smaller);
    }

    return answer;
}

long long solve_even_case(const vector<int>& digits) {
    const int n = static_cast<int>(digits.size());
    const int full_mask = (1 << n) - 1;
    const int length = n / 2;

    vector<long long> powers(length + 1, 1);
    for (int i = 1; i <= length; ++i) {
        powers[i] = powers[i - 1] * 10;
    }

    vector<array<long long, 3>> memo(1 << n);
    vector<array<bool, 3>> seen(1 << n);

    function<long long(int, int)> dfs = [&](int mask, int cmp) -> long long {
        const int state = cmp + 1;
        if (mask == 0) {
            return 0;
        }
        if (seen[mask][state]) {
            return memo[mask][state];
        }
        seen[mask][state] = true;

        const int remaining_digits = bit_count(mask);
        const int remaining_positions = remaining_digits / 2;
        const long long place_value = powers[remaining_positions - 1];
        const bool first_position = (remaining_digits == n);

        long long best = (cmp == 1 || cmp == 0) ? INF : -INF;

        for (int i = 0; i < n; ++i) {
            if ((mask & (1 << i)) == 0) {
                continue;
            }
            if (first_position && length > 1 && digits[i] == 0) {
                continue;
            }
            for (int j = 0; j < n; ++j) {
                if (i == j || (mask & (1 << j)) == 0) {
                    continue;
                }
                if (first_position && length > 1 && digits[j] == 0) {
                    continue;
                }

                const int next_mask = mask ^ (1 << i) ^ (1 << j);
                const long long current = 1LL * (digits[i] - digits[j]) * place_value;

                if (cmp == 1) {
                    best = min(best, current + dfs(next_mask, 1));
                } else if (cmp == -1) {
                    best = max(best, current + dfs(next_mask, -1));
                } else {
                    const int next_cmp = (digits[i] > digits[j] ? 1 : (digits[i] < digits[j] ? -1 : 0));
                    const long long candidate = current + dfs(next_mask, next_cmp);
                    if (abs(candidate) < abs(best) ||
                        (abs(candidate) == abs(best) && candidate < best)) {
                        best = candidate;
                    }
                }
            }
        }

        memo[mask][state] = best;
        return best;
    };

    return abs(dfs(full_mask, 0));
}

long long solve_case(vector<int> digits) {
    if (digits.size() % 2 == 1) {
        return solve_odd_case(digits);
    }
    return solve_even_case(digits);
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> digits(n);
        for (int i = 0; i < n; ++i) {
            cin >> digits[i];
        }
        cout << solve_case(digits) << '\n';
    }

    return 0;
}
