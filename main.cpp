#include <algorithm>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<pair<int, int>> points(n);
    vector<int> xs, ys;
    xs.reserve(n);
    ys.reserve(n);
    for (auto& [x, y] : points) {
        cin >> x >> y;
        xs.push_back(x);
        ys.push_back(y);
    }

    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    const int xCount = static_cast<int>(xs.size());
    const int yCount = static_cast<int>(ys.size());

    vector<vector<int>> prefix(xCount + 1, vector<int>(yCount + 1, 0));
    for (const auto& [x, y] : points) {
        int xi = static_cast<int>(lower_bound(xs.begin(), xs.end(), x) - xs.begin());
        int yi = static_cast<int>(lower_bound(ys.begin(), ys.end(), y) - ys.begin());
        ++prefix[xi + 1][yi + 1];
    }

    for (int i = 1; i <= xCount; ++i) {
        for (int j = 1; j <= yCount; ++j) {
            prefix[i][j] += prefix[i - 1][j] + prefix[i][j - 1] - prefix[i - 1][j - 1];
        }
    }

    auto countPoints = [&](int lx, int rx, int ly, int ry) -> int {
        if (lx > rx || ly > ry) {
            return 0;
        }
        ++rx;
        ++ry;
        return prefix[rx][ry] - prefix[lx][ry] - prefix[rx][ly] + prefix[lx][ly];
    };

    auto area = [&](int lx, int rx, int ly, int ry) -> long long {
        return 1LL * (xs[rx] - xs[lx]) * (ys[ry] - ys[ly]);
    };

    unordered_map<long long, long long> memo;
    const long long keyBase = max(xCount, yCount) + 1LL;

    function<long long(int, int, int, int, int)> solve =
        [&](int lx, int rx, int ly, int ry, int rects) -> long long {
            if (countPoints(lx, rx, ly, ry) == 0) {
                return 0;
            }

            while (lx <= rx && countPoints(lx, lx, ly, ry) == 0) {
                ++lx;
            }
            while (lx <= rx && countPoints(rx, rx, ly, ry) == 0) {
                --rx;
            }
            while (ly <= ry && countPoints(lx, rx, ly, ly) == 0) {
                ++ly;
            }
            while (ly <= ry && countPoints(lx, rx, ry, ry) == 0) {
                --ry;
            }

            if (lx > rx || ly > ry) {
                return 0;
            }

            long long best = area(lx, rx, ly, ry);
            if (rects == 1 || best == 0) {
                return best;
            }

            long long key = rects;
            key = key * keyBase + lx;
            key = key * keyBase + rx;
            key = key * keyBase + ly;
            key = key * keyBase + ry;

            if (auto it = memo.find(key); it != memo.end()) {
                return it->second;
            }

            for (int cut = lx; cut < rx; ++cut) {
                if (countPoints(lx, cut, ly, ry) == 0 || countPoints(cut + 1, rx, ly, ry) == 0) {
                    continue;
                }
                for (int leftRects = 1; leftRects < rects; ++leftRects) {
                    best = min(best,
                               solve(lx, cut, ly, ry, leftRects) +
                                   solve(cut + 1, rx, ly, ry, rects - leftRects));
                }
            }

            for (int cut = ly; cut < ry; ++cut) {
                if (countPoints(lx, rx, ly, cut) == 0 || countPoints(lx, rx, cut + 1, ry) == 0) {
                    continue;
                }
                for (int bottomRects = 1; bottomRects < rects; ++bottomRects) {
                    best = min(best,
                               solve(lx, rx, ly, cut, bottomRects) +
                                   solve(lx, rx, cut + 1, ry, rects - bottomRects));
                }
            }

            memo[key] = best;
            return best;
        };

    cout << solve(0, xCount - 1, 0, yCount - 1, k) << '\n';
    return 0;
}
