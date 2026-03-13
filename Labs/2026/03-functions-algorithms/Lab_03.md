---
marp: true
size: 16:9
style: |
    img[alt~="center"] {
      display: block;
      margin: 0 auto;
    }
---

# Laboratory 03
## Functional Programming and Algorithms

### Paolo Joseph Baioni
[paolojoseph.baioni@polimi.it](mailto:paolojoseph.baioni@polimi.it)
### 13/03/2026

---
## Outline
1. I/O in C++
2. `std::vector` & `std::map`
3. `std::chrono`
4. JSON and header-only external libraries
5. Command-line arguments
6. Lambdas
7. Exercise: Newton
8. Break
---
## Outline
9. Iterators
10. Standard algorithms
11. Execution policies
12. Ranges
13. Counting iterators
14. Exercise: saxpy and bandwidth
15. Extras (muParser, GetPot, Horner)

---
## 1. I/O in C++

To handle files in C++ we `#include <fstream>`, for example, to open a file

```cpp
std::ifstream input_file("input_datafile");

if (!input_file) {
    std::cerr << "Some error message\n";
    return 1;
}
```
---
## 1. I/O in C++
we can then read contents, for example line by line with
```cpp

std::string line;
while (std::getline(input_file, line)) {
    std::cout << line << std::endl;
}
```
> RAII: when out of scope, fstream object will automatically close

---
## 1. I/O in C++
Similar holds for output, we can also specify modes
```cpp
std::ofstream output_file("output_textfile.txt", std::ios::out | std::ios::app);

if (!output_file) {
    std::cerr << "Some error message\n";
    return 1;
}

output_file << "Appending a new line, possibly creating file" << std::endl;

```

---
## Example: parameters file and compilation
We can use parameters files to change program behaviour w/o recompiling it
```cpp
std::ifstream param_file("parameters.txt");

if (!param_file) {...}

unsigned row;
param_file >> row;

for (unsigned i=0; std::getline(input_file, line); ++i) 
    if (i==row) 
        output_file << "line " << row << " is: " << line << std::endl;
```
> Snippets are independent examples, if you want to run them sequentially, you must reset the input stream
> Check also `param_file >> row;`: `if (!(param_file >> row)) {...}`

---
## 2. std::vector
Sequential container -- remember about `reserve()` and `resize()`
```cpp
std::vector <int> v(10);
v[5]=1; // O(1) - random access
```

---
## 2. std::map
Associative container
```cpp
std::map <int, std::string> m; //dictionary
m[1]="uno"; // O(logn) - create
std::cout << m[1] << std::endl; //O(logn) - access
```

---
## 3. std::chrono
```cpp
std::map<int, int> m;
auto start = std::chrono::high_resolution_clock::now();
for (int i = 0; i < (1 << 20); ++i) 
    m[i] = i;
auto end = std::chrono::high_resolution_clock::now();
auto delta_m = std::chrono::duration_cast<
std::chrono::microseconds>(end - start).count ();

std::unordered_map<int, int> u; 
u.reserve(1 << 20); //prevents repeated rehashing: try to comment this
start = std::chrono::high_resolution_clock::now();
for (int i = 0; i < (1 << 20); ++i) 
    u[i] = i;
end = std::chrono::high_resolution_clock::now();
auto delta_u = std::chrono::duration_cast<
std::chrono::microseconds>(end - start).count ();

std::cout << "speed-up: " << (static_cast<double>(delta_m)
/static_cast<double>(delta_u)) << std::endl;
```

---
## 4. JSON
JSON (JavaScript Object Notation) is a lightweight, language-independent and standardised text format for representing structured data using key–value pairs and arrays.
 
 > nlohmann JSON Library
 > Boost JSON Library

see files in `./JSON_example`

---
## 4. Header-only libraries
When dealing with header-only libraries, it is enough to include them.
If the path is non standard, you can either specify the full path, possibly wrt the current position, with `#include ""`, or use the standard `#include <>` and specify the path with `-I` when calling `g++`.

When you write your own header, it is important to avoid multiple definition: use either `#pragma once` or `#ifndef` header guards.

 > Check what happens with a `hello.h`

---
## 4. A note on CMake

Clone the library (header-only):

```bash
git clone --depth=1 https://github.com/nlohmann/json.git
```

Project structure (example):

```
today/
├── JSON_example/
│   ├── example.cpp
│   ├── input.json
│   └── output.json
└── json/   ← cloned repo
```
---
CMakeLists.txt (inside `JSON_example/`):

```cmake
cmake_minimum_required(VERSION 3.20)
project(example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example example.cpp)

target_include_directories(example PRIVATE
    ${PROJECT_SOURCE_DIR}/../json/single_include/nlohmann
)
```
---
Build (from `JSON_example/`):

```bash
mkdir build && cd build
cmake ..
make
```

---
## 5. Command-line arguments

`int main(int argc, char* argv[])`

- `argc` = number of arguments (including program name)
- `argv` = array of C-strings
- `argv[0]` is the program name

Example: `./program input.txt 10`

> Convert arguments with `std::stoi()` and `std::stod()`
> Extra: external libraries (see last slide)


---
## 6. Lambdas
Lambdas are anonymous functions, such as
```cpp
auto add_integers = [](int a, int b) { return a + b; };
```
We can capture by reference and value with:
```cpp
std::vector<int> v = {0, 1, 2, 3};
int a = 4;

auto f = [&v, a](int i) { return v[i] * a; };

std::cout << "4 = " << f(1) << std::endl;
```
> For GPU offloading, capture `v=v.data()` instead of `&v`
---
## 6. Lambdas

Equivalent to a compiler-generated function object (callable struct, functor):

```cpp
struct __unnamed {
    int a;
    std::vector<int>& v;

    int operator()(int i) const {
        return v[i] * a;
    }
};

__unnamed f{a, v};
std::cout << "4 = " << f(1) << std::endl;
```
---
## 7. Newton

1. Implement a `NewtonSolver` class.
2. Write different `main` files that pass functions and derivatives as:
   1. Function pointers;
   2. Lambda functions;
   3. Callable structs;
   4. Extra: `muParser` functions  
      (load with `spack load muparser` on the cluster, or install with `./install_PACS.sh` from `${PACS_ROOT}/Extras/muParser`);
   5. Extra: Let the user pass the function and parameters from the command line using `GetPot` and `muParser`.
3. Use the solver to solve ` x^3 + 5x + 3 = 0 `

---
# 8. Break


---
## 9. Iterators
**Generic programming** separates algorithms from data structures.

Iterators provide a **uniform interface** to access container elements.

This allows:
- Writing an algorithm once
- Reusing it with `vector`, `list`, ... custom containers
- Achieving type safety at compile time

A pointer is a type of iterator.

⚠ Iterators must always be valid — dereferencing an invalid iterator is undefined behavior.

---
## 9. Iterators
Iterators behave like pointers:

```cpp
for (auto it = container.begin();
     it != container.end();
     ++it)
{
    // use *it
}
```

Why `!=` instead of `<`?

* `!=` works for **all iterator categories**
* `<` works only for random-access iterators

### This makes the loop fully generic.

---
## 10. Standard algorithms
STL algorithms provide tested, optimised implementations  for common data-processing patterns.

Instead of:

```cpp
for (size_t i = 0; i < v.size(); ++i)
    v[i] = f(i);
```

We write:

```cpp
std::for_each(first, last, f);
```
---
## 10. Standard algorithms
Algorithms:

* are container-independent
* describe *what*, not *how*
* are often better optimised than hand-written loops


---
## 11. Execution policies
Many algorithms accept an optional **execution policy** as first argument:

```cpp
std::for_each(std::execution::seq,  first, last, f);
std::for_each(std::execution::par,  first, last, f);
std::for_each(std::execution::par_unseq, first, last, f);
std::for_each(std::execution::unseq, first, last, f); // C++20
```

Execution policies specify the **allowed parallelism model**.

---
## 11. Execution policies

- **seq**  
  Sequential execution (no parallelism).

- **par**  
  Parallel execution on multiple threads.  
  Iterations may run in different threads.

- **par_unseq**  
  Parallel + vectorized execution.  
  Iterations may run:
  - on different threads
  - unsequenced within each thread.

---
## 11. Execution policies 

- **unseq** (C++20)  
  Vectorized execution on a single thread.

⚠ With parallel policies, the programmer must avoid:
- data races
- undefined behaviour
- inter-iteration dependencies

---
## 12. Ranges (C++20)

Ranges provide a standard way to generate and manipulate sequences.

Example: generate indices without allocating memory

```cpp
auto indices = std::views::iota(0, n);

std::for_each(indices.begin(), indices.end(),f);
```

Requires:

```cpp
#include <ranges>
```

`std::views::iota` replaces explicit index vectors.



---
## 13. Counting iterators

Before C++20 ranges, we used counting iterators.

Boost example:

```cpp
boost::counting_iterator<std::size_t> first(0), last(n);

std::for_each(first, last, f);
```
---
## 13. Counting iterators
Advantages:

* no index vector allocation
* works with execution policies
* fully compatible with `std::for_each`

Counting iterators and ranges both model a virtual sequence of indices.

---
## 14. Exercise: saxpy and bandwidth

SAXPY is a **memory-bound** kernel (very low arithmetic intensity).

If you run the program with increasing vector sizes (e.g. powers of two), you should observe:

- **Small sizes** → poor performance  
  → parallel overhead dominates (threading, scheduling cost > useful work).

- **Intermediate sizes** → peak performance  
  → data still fits in cache, high effective bandwidth, good vectorization.

- **Large sizes** → performance decreases and then stabilizes  
  → working set exceeds cache capacity, computation becomes limited by memory bandwidth.

---
## 15. Extras

Check:

- [2025/Lab_03](https://github.com/pacs-course/pacs-Labs/tree/main/Labs/2025/03-functional) for muParser, GetPot, and Horner rule exercises and examples.

- [2025/Lab_04](https://github.com/pacs-course/pacs-Labs/tree/main/Labs/2025/04-algorithms_and_execution_policies) for offloading computation to the GPU with `for_each` and other algorithms  -- you can `spack load nvhpc` on the cluster

- [cppreference](https://en.cppreference.com/w/cpp/algorithm.html)  for the full list of standard algorithms.

- [Thrust](https://docs.nvidia.com/cuda/archive/11.8.0/pdf/Thrust_Quick_Start_Guide.pdf)  for a counting-iterator and parallel-algorithm approach designed for GPUs.

