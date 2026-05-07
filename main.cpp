#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  long long n, goal;
  if (!(std::cin >> n >> goal)) {
    return 0;
  }

  if (n <= 0) {
    std::cerr << "Invalid input: n must be positive.\n";
    return 1;
  }

  std::vector<long long> machines(n);
  for (long long i = 0; i < n; ++i) {
    std::cin >> machines[i];
    if (machines[i] <= 0) {
      std::cerr << "Invalid input: machine times must be positive.\n";
      return 1;
    }
  }

  if (goal < 0) {
    std::cerr << "Invalid input: goal must be non-negative.\n";
    return 1;
  }
  if (goal == 0) {
    std::cout << 0 << '\n';
    return 0;
  }

  long long min_machine = *std::min_element(machines.begin(), machines.end());
  long long low = 1;
  long long high = (min_machine > LLONG_MAX / goal) ? LLONG_MAX : min_machine * goal;
  long long answer = high;

  while (low <= high) {
    long long mid = low + (high - low) / 2;
    long long produced = 0;

    for (long long machine : machines) {
      long long add = mid / machine;
      if (produced >= goal - add) {
        produced = goal;
        break;
      }
      produced += add;
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
