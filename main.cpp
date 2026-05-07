#include <algorithm>
#include <functional>
#include <iostream>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> values;
    for (int x; cin >> x;) {
        values.push_back(x);
    }

    if (values.empty()) {
        return 0;
    }

    int n = values[0];
    if (n <= 0) {
        cout << 0 << '\n';
        return 0;
    }
    int edge_count = n > 0 ? n - 1 : 0;
    int start = 1;

    if (static_cast<int>(values.size()) >= 2 && static_cast<int>(values.size()) == 2 + values[1] * 2) {
        edge_count = values[1];
        start = 2;
    } else if (static_cast<int>(values.size()) == 1 + (n - 1) * 2) {
        edge_count = n - 1;
        start = 1;
    }

    vector<vector<int>> graph(n + 1);
    for (int i = 0; i < edge_count; ++i) {
        int u = values[start + i * 2];
        int v = values[start + i * 2 + 1];
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    vector<int> parent(n + 1, 0), depth(n + 1, 0), order;
    vector<vector<int>> children(n + 1);
    order.reserve(n);

    stack<int> st;
    st.push(1);
    parent[1] = -1;
    depth[1] = 1;
    while (!st.empty()) {
        int u = st.top();
        st.pop();
        order.push_back(u);
        for (int v : graph[u]) {
            if (v == parent[u]) {
                continue;
            }
            parent[v] = u;
            depth[v] = depth[u] + 1;
            children[u].push_back(v);
            st.push(v);
        }
    }

    vector<int> subtree_size(n + 1, 1);
    int max_depth = 1;
    for (int i = static_cast<int>(order.size()) - 1; i >= 0; --i) {
        int u = order[i];
        max_depth = max(max_depth, depth[u]);
        for (int v : children[u]) {
            subtree_size[u] += subtree_size[v];
        }
    }

    vector<vector<int>> nodes_at_depth(max_depth + 1);
    for (int u = 1; u <= n; ++u) {
        nodes_at_depth[depth[u]].push_back(u);
    }

    vector<int> tin(n + 1, 0), tout(n + 1, 0), euler;
    euler.reserve(n);
    int timer = 0;
    function<void(int)> build_euler_tour = [&](int u) {
        tin[u] = timer++;
        euler.push_back(u);
        for (int v : children[u]) {
            build_euler_tour(v);
        }
        tout[u] = timer - 1;
    };
    build_euler_tour(1);

    for (int d = 1; d <= max_depth; ++d) {
        sort(nodes_at_depth[d].begin(), nodes_at_depth[d].end(), [&](int lhs, int rhs) {
            return subtree_size[lhs] > subtree_size[rhs];
        });
    }

    vector<char> blocked(n + 1, false);
    int answer = n;

    function<void(int, int)> search_optimal_cut = [&](int current_depth, int infected_count) {
        if (infected_count >= answer) {
            return;
        }
        if (current_depth > max_depth) {
            answer = min(answer, infected_count);
            return;
        }

        vector<int> candidates;
        for (int u : nodes_at_depth[current_depth]) {
            if (!blocked[u]) {
                candidates.push_back(u);
            }
        }

        if (candidates.empty()) {
            answer = min(answer, infected_count);
            return;
        }

        if (candidates.size() == 1) {
            answer = min(answer, infected_count);
            return;
        }

        infected_count += static_cast<int>(candidates.size()) - 1;
        for (int u : candidates) {
            vector<int> changed;
            for (int idx = tin[u]; idx <= tout[u]; ++idx) {
                int node = euler[idx];
                if (!blocked[node]) {
                    blocked[node] = true;
                    changed.push_back(node);
                }
            }
            search_optimal_cut(current_depth + 1, infected_count);
            for (int node : changed) {
                blocked[node] = false;
            }
        }
    };

    search_optimal_cut(2, 1);
    cout << answer << '\n';
    return 0;
}
