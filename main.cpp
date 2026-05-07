#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  long long n, goal;
  if (!(std::cin >> n >> goal)) {
    return 0;
  }

  std::vector<long long> machines(n);
  for (long long i = 0; i < n; ++i) {
    std::cin >> machines[i];
  }

  long long min_machine = *std::min_element(machines.begin(), machines.end());
  long long low = 1;
  long long high = min_machine * goal;
  long long answer = high;

  while (low <= high) {
    long long mid = low + (high - low) / 2;
    __int128 produced = 0;

    for (long long machine : machines) {
      produced += mid / machine;
      if (produced >= goal) break;
    }

    if (produced >= goal) {
      answer = mid;
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }

  std::cout << answer << '\n';
  return 0;
}
