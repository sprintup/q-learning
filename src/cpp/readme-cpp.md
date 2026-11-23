> AI-generated study note — not part of the graded write-up.

# Setting up the C++ A* project (step by step)

This guide walks you through building `src\cpp` yourself, modeled on the homework repo in
`src\cu-cspb-2270-fall-2026-classroom-hw-05-priority-queue-sprintup` (called "the HW repo" below).
It tells you what to create and what commands to run. It does not write the A* code for you.

## 0. What the HW repo actually uses

| Piece | HW repo | What we'll do here |
|---|---|---|
| Build system | CMake (`CMakeLists.txt`) | Same |
| Test framework | GoogleTest, downloaded at configure time via `CMakeLists.txt.in` + `ExternalProject` (pinned to `release-1.10.0`) | GoogleTest via CMake's newer `FetchContent` (one file, no `.in` file) |
| Layout | `code/` (your `.h`/`.cpp`), `app/main.cpp`, `tests/test_*.cpp` | Same |
| Executables | `run_tests` (tests + `code/`), `run_app` (`app/main.cpp` + `code/`) | Same |
| Compiler flags | `-std=c++11 -Wall`, Debug by default | `-std=c++17 -Wall`, Debug by default |
| Editor | VS Code: `tasks.json` runs `cd build && cmake .. && make`, `launch.json` debugs with gdb | Same idea, but with Windows-friendly commands |
| Build output | `build/` (git-ignored) | Same |

Two things in the HW repo won't work as-is on your Windows machine, which is why this guide changes them:

1. **It's written for Linux/macOS.** `tasks.json` uses `rm -rf` and `make`, and `launch.json` points at `/usr/bin/gdb`.
2. **It's old CMake.** `cmake_minimum_required(VERSION 2.8.11)` and GoogleTest 1.10.0 both declare
   CMake versions below 3.5, and **CMake 4.x refuses to configure those**. If you install a current
   CMake, copying the HW repo's files verbatim will fail with an error like
   `Compatibility with CMake < 3.5 has been removed`. Using `cmake_minimum_required(VERSION 3.14)` and a
   newer GoogleTest fixes that.

## 1. Install the toolchain (one time)

The HW repo assumes gcc + gdb, so the closest match on Windows is **MSYS2 (UCRT64)**, which gives you
gcc, g++, gdb, CMake, and Ninja.

1. Install MSYS2 from <https://www.msys2.org/> (default location `C:\msys64`), or with winget in PowerShell:
   ```powershell
   winget install MSYS2.MSYS2
   ```
2. Open the **"MSYS2 UCRT64"** terminal from the Start menu and install the tools:
   ```bash
   pacman -Syu        # it may close the window; reopen "MSYS2 UCRT64" and run it again
   pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
   ```
3. Add `C:\msys64\ucrt64\bin` to your Windows **user PATH** (Start → "Edit environment variables for
   your account" → `Path` → New). Then open a **new** PowerShell window and check:
   ```powershell
   g++ --version
   gdb --version
   cmake --version
   ninja --version
   ```
   All four should print a version. If one says "not recognized", PATH isn't set or you need a new window.

> Alternatives: WSL (Ubuntu) lets you use the HW repo's files nearly unchanged (`sudo apt install build-essential cmake gdb`),
> and Visual Studio's "Desktop development with C++" works too, but then `-Wall` behaves differently and
> the debugger config is different. The rest of this guide assumes MSYS2.

## 2. Create the folder layout

From the repo root in PowerShell:

```powershell
cd C:\Users\Work\source\repos\q-learning\src\cpp
mkdir app, code, tests, .vscode
New-Item app\main.cpp, code\AStar.h, code\AStar.cpp, tests\test_a_star.cpp, CMakeLists.txt, .gitignore -ItemType File
```

You should end up with:

```
src\cpp\
├── readme-cpp.md          (this file)
├── CMakeLists.txt
├── .gitignore
├── .vscode\
│   ├── tasks.json
│   └── launch.json
├── app\
│   └── main.cpp           (a playground, like the HW repo's main.cpp)
├── code\
│   ├── AStar.h            (your declarations)
│   └── AStar.cpp          (your implementation)
└── tests\
    └── test_a_star.cpp    (GoogleTest tests)
```

File names are up to you. `CMakeLists.txt` below just globs whatever is in `code/` and `tests/test_*.cpp`,
same as the HW repo.

## 3. Write `.gitignore`

Keep the build output out of git, like the HW repo does:

```gitignore
build/
.vscode/ipch
```

## 4. Write `CMakeLists.txt`

Here is the structure, mirroring the HW repo section by section. Read each comment and compare it
against the HW repo's file so you know what each part does.

```cmake
cmake_minimum_required(VERSION 3.14)          # 3.14+ is needed for FetchContent_MakeAvailable
project(a_star_cpp LANGUAGES CXX)

# Debug by default (same as the HW repo)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Release or Debug" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)                    # replaces -std=c++11 in the HW repo
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall)

# Your source files and test files (same globs as the HW repo)
file(GLOB USER_FILES "code/*.h" "code/*.cpp")
file(GLOB TEST_FILES "tests/test_*.cpp")

# GoogleTest: replaces the HW repo's CMakeLists.txt.in + ExternalProject block
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)   # same Windows setting as the HW repo
FetchContent_MakeAvailable(googletest)

# run_tests = tests + your code   (same as the HW repo)
add_executable(run_tests ${TEST_FILES} ${USER_FILES})
target_link_libraries(run_tests GTest::gtest_main)

# run_app = app/main.cpp + your code   (same as the HW repo)
add_executable(run_app app/main.cpp ${USER_FILES})

# Optional: lets `ctest` find each test
enable_testing()
include(GoogleTest)
gtest_discover_tests(run_tests)
```

Notes:
- GoogleTest 1.17 requires C++17, which is why the standard is 17 here.
- You don't need to link `pthread` yourself the way the HW repo does; the `GTest::gtest_main` target handles it.
- `file(GLOB ...)` only re-scans when CMake re-configures. If you add a new `.cpp` file, re-run the
  configure command in step 6.

## 5. Plan the code (you write this part)

### Interface: mirror the Python version

Your Python function is `aStarAlgorithm(startRow, startCol, endRow, endCol, graph)`, where `graph` is
a grid of `0` (open) and `1` (wall), moves are up/down/left/right, and it returns a list of `[row, col]`
pairs from start to end, or `[]` if the end can't be reached. A direct C++ equivalent for `code/AStar.h` is:

```cpp
std::vector<std::vector<int>> aStarAlgorithm(int startRow, int startCol,
                                             int endRow, int endCol,
                                             const std::vector<std::vector<int>>& graph);
```

Taking `graph` by `const&` gives you the "does not modify the input graph" test for free.

### Things to decide as you translate from Python

- **`Node`**: a `struct` with `row`, `col`, `value`, `g` (distance from start), `f` (estimated total),
  and `cameFrom`. In C++, `cameFrom` can be a `Node*` (pointing into your 2D `std::vector<Node>`
  grid) or a pair of indices.
- **Infinity**: your README mentions wanting to figure this out. Look up
  `std::numeric_limits<int>::max()` (in `<limits>`) or
  `std::numeric_limits<double>::infinity()`.
- **The min-heap**: two options.
  1. Write your own min-heap like the Python version (with `contains` / `update`). Your HW-05
     priority queue is a good starting point. This fits the course, since the README has to name a
     data structure you implemented.
  2. Use `std::priority_queue` with `std::greater<>`. It has no "update", so the usual trick is to
     push a duplicate entry and skip stale ones when you pop them.
- **Reconstructing the path**: follow `cameFrom` back from the end, then `std::reverse` it (the C++
  version of Python's `[::-1]`).

## 6. Build and run from PowerShell

From `src\cpp`:

```powershell
# configure (first time, and after adding new files). Downloads GoogleTest the first time.
cmake -S . -B build -G Ninja

# build both executables
cmake --build build

# run the tests
.\build\run_tests.exe

# run your playground main.cpp
.\build\run_app.exe
```

Useful extras:

```powershell
.\build\run_tests.exe --gtest_filter=*Unreachable*   # run only matching tests
ctest --test-dir build --output-on-failure           # run via CTest instead
Remove-Item -Recurse -Force build                    # clean rebuild (PowerShell's "rm -rf build")
```

Before writing any A*, a good first check is an empty `main()` plus one trivial test
(`TEST(Sanity, Works) { EXPECT_EQ(1 + 1, 2); }`), so you know the toolchain works before you start
debugging your own code.

## 7. Port the Python tests to GoogleTest

`src\python\test_a_star.py` has 8 grid cases plus 3 extra tests. A plan for `tests/test_a_star.cpp`:

1. `#include <gtest/gtest.h>` and `#include "../code/AStar.h"` (same pattern as the HW repo's `test_hw.cpp`).
2. Copy the 8 grids over. Python's `[[0, 1], [1, 0]]` becomes C++'s
   `std::vector<std::vector<int>> g = {{0, 1}, {1, 0}};`, so it's mostly swapping `[]` for `{}`.
3. Write a `bfs_shortest_length` helper (use `std::queue`) and a `check_path` helper that does the same
   checks as the Python one: starts at start, ends at end, never on a wall, every step is one
   up/down/left/right move, and the length matches BFS.
4. Write one `TEST(...)` per case, or put the cases in a `std::vector` of structs and use
   GoogleTest's `TEST_P` / `INSTANTIATE_TEST_SUITE_P` (the C++ version of `@pytest.mark.parametrize`).
5. Add the 3 extra tests: unreachable end returns empty, straight line with no walls, input graph unchanged.

Useful assertions: `EXPECT_EQ`, `ASSERT_FALSE(path.empty())`, `EXPECT_TRUE`. Use `ASSERT_*` when the
rest of the test would crash if it failed (for example, indexing `path[0]` on an empty path).

## 8. (Optional) VS Code build and debug

Same idea as the HW repo's `.vscode` files, adapted for Windows and MSYS2. Install the **C/C++**
extension (ms-vscode.cpptools). Open `src\cpp` as the folder in VS Code so `${workspaceFolder}` points there.

`.vscode/tasks.json` (replaces `cd build && cmake .. && make`):

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "build project",
      "type": "shell",
      "command": "cmake -S . -B build -G Ninja; cmake --build build",
      "options": { "cwd": "${workspaceFolder}" },
      "group": { "kind": "build", "isDefault": true },
      "problemMatcher": ["$gcc"]
    }
  ]
}
```

`.vscode/launch.json` (same two configs as the HW repo, pointing at the `.exe` and MSYS2's gdb):

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Run Tests",
      "type": "cppdbg",
      "request": "launch",
      "preLaunchTask": "build project",
      "program": "${workspaceFolder}/build/run_tests.exe",
      "cwd": "${workspaceFolder}/build",
      "MIMode": "gdb",
      "miDebuggerPath": "C:/msys64/ucrt64/bin/gdb.exe",
      "externalConsole": false
    },
    {
      "name": "Run App",
      "type": "cppdbg",
      "request": "launch",
      "preLaunchTask": "build project",
      "program": "${workspaceFolder}/build/run_app.exe",
      "cwd": "${workspaceFolder}/build",
      "MIMode": "gdb",
      "miDebuggerPath": "C:/msys64/ucrt64/bin/gdb.exe",
      "externalConsole": false
    }
  ]
}
```

Then `Ctrl+Shift+B` builds, and F5 (after picking "Run Tests" or "Run App" in the Run and Debug panel)
builds and starts the debugger, so you can set breakpoints in `AStar.cpp`.

## 9. Troubleshooting

| Symptom | Likely cause / fix |
|---|---|
| `Compatibility with CMake < 3.5 has been removed` | You copied the HW repo's `cmake_minimum_required(2.8...)` or GoogleTest 1.10. Use the step 4 versions. |
| `'g++' / 'cmake' is not recognized` | `C:\msys64\ucrt64\bin` isn't on PATH, or the PowerShell window was opened before you changed PATH. |
| `CMake was unable to find a build program corresponding to "Ninja"` | Install `mingw-w64-ucrt-x86_64-ninja` (step 1). |
| Generator mismatch error after switching generators | Delete `build\` and configure again. |
| New `.cpp` file "not found" at link time | Re-run `cmake -S . -B build -G Ninja` so the `GLOB` picks it up. |
| GoogleTest download fails | Needs internet the first time; after that it's cached in `build\_deps`. |

## 10. Checklist

- [ ] MSYS2 installed; `g++`, `gdb`, `cmake`, `ninja` all print versions in PowerShell
- [ ] Folder layout created (step 2)
- [ ] `.gitignore` and `CMakeLists.txt` written
- [ ] Sanity test builds and passes
- [ ] `AStar.h` interface written
- [ ] Python tests ported to `tests/test_a_star.cpp`
- [ ] A* implemented and all tests pass
- [ ] (Optional) VS Code tasks and debugging working
