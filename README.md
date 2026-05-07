# algorithm

## C++ solution (minimum time needed)

This repository now includes a fast C++ solution for finding the minimum time
needed to finish a target number of tasks when each machine has its own task
time.

### Input format

- `n goal`
- `n` integers where each value is the time one machine needs for one task

### Output format

- One integer: the minimum time required to produce at least `goal` tasks

### Build and run

```bash
g++ -O2 -std=c++17 /home/runner/work/algorithm/algorithm/main.cpp -o /tmp/algorithm_main
echo -e "3 10\n2 3 7" | /tmp/algorithm_main
```
