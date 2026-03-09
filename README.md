# CS4560 Assignment 1

This repository contains a C++/OpenMP implementation of a min/max search over a randomly generated 3D matrix. The project includes one sequential baseline and three parallel variants, along with a `Makefile` for building all executables.

The code uses a `1000 x 1000 x 1000` integer matrix generated with a fixed random seed, then reports:

- total elapsed time
- global minimum value and its coordinates
- global maximum value and its coordinates

## Repository structure

- `sp.cpp` — sequential implementation
- `pp1.cpp` — parallel implementation using an OpenMP `parallel` region with a collapsed `for` loop and thread-local min/max values merged in a `critical` section
- `pp2.cpp` — parallel implementation using OpenMP `sections`, with one section computing the minimum and the other computing the maximum
- `pp3.cpp` — parallel implementation using nested parallelism: outer sections split min/max work, while inner parallel loops search each section’s assigned result
- `config.hpp` — shared configuration, matrix allocation, and deterministic random input generation
- `Makefile` — build rules for all targets
- `Assignment_1.pdf` — assignment handout

## Requirements

- C++17-compatible compiler
- OpenMP support
- `make`

Tested build flags in the repository enable:

- `-O2`
- `-std=c++17`
- `-Wall -Wextra -pedantic`
- `-fopenmp`

## Build

To compile all programs:

```bash
make
```

This produces four executables:

- `sp`
- `pp1`
- `pp2`
- `pp3`

To remove compiled binaries:

```bash
make clean
```

## Run

Run each version directly from the repository root:

```bash
./sp
./pp1
./pp2
./pp3
```

Each program prints output in the form:

```text
Elapsed: <seconds> s
Min: <value> at (<i>,<j>,<k>)
Max: <value> at (<i>,<j>,<k>)
```

## Implementation summary

### Shared configuration

`config.hpp` defines the problem size using constants:

```cpp
M = 1000;
N = 1000;
P = 1000;
```

It also provides:

- `create_matrix()` to allocate the 3D container
- `read_input(...)` to fill the matrix with pseudo-random integers in the range `[0, 100]`

Because the generator uses a fixed seed by default, all program variants operate on comparable input data.

### Sequential baseline (`sp.cpp`)

The sequential version scans the entire 3D matrix in triple nested loops and updates the current min/max positions as it traverses the data.

### Parallel version 1 (`pp1.cpp`)

This version:

- sets a fixed OpenMP thread count
- uses `#pragma omp parallel`
- distributes iterations with `#pragma omp for collapse(3)`
- stores per-thread local min/max values
- combines thread-local results inside a `critical` section

This is the most direct data-parallel version of the baseline search.

### Parallel version 2 (`pp2.cpp`)

This version uses:

- `#pragma omp parallel`
- `#pragma omp sections`

One section scans the whole matrix for the minimum, while the other scans the whole matrix for the maximum. This separates the two reductions conceptually, but each section still traverses the full dataset.

### Parallel version 3 (`pp3.cpp`)

This version enables nested OpenMP parallelism and combines:

- outer `sections` to split min and max work
- inner parallel loops to accelerate each search independently

Conceptually, this is a hierarchical parallel design: one level partitions the task by objective (min vs. max), and a second level parallelizes the traversal within each objective.

## Notes

- The matrix is extremely large, so runtime and memory footprint can be significant depending on the system.
- The project uses `std::vector<std::vector<std::vector<int>>>`, which is convenient but may not be cache-optimal compared with a flat contiguous allocation.
- Thread counts are hard-coded in the current source files (`pp1.cpp`, `pp2.cpp`, `pp3.cpp`).
- Since all variants print min/max coordinates, you can verify correctness by comparing outputs across executables.

## Suggested evaluation workflow

A simple way to compare implementations is:

```bash
make
./sp
./pp1
./pp2
./pp3
```

Record elapsed times for each executable and compare:

- sequential baseline vs parallel speedup
- different OpenMP strategies
- impact of nested parallelism relative to simpler parallel forms

## Possible future improvements

- accept matrix dimensions and thread counts from the command line
- flatten the 3D storage into a 1D contiguous buffer for better locality
- replace `critical` updates with reduction-style logic where appropriate
- run multiple trials and report average timing
- add automated validation to ensure all versions return identical min/max results

## License

No license file is currently included in the repository. All rights remain with the repository owner unless a license is added.