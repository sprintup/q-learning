> AI-generated study note — not part of the graded write-up.

# Setting up the C++ A* project in WSL (step by step)

This guide walks you through building `src/cpp` yourself, modeled on the homework repo in
`src/hw-05` (called "the HW repo" below).
It uses **WSL (Ubuntu)**. The HW repo was written for Linux, so in WSL you can follow it almost
exactly. This guide tells you what to create and which commands to run. It does not write the A* code for you.

## 0. What the HW repo actually uses

| Piece | HW repo | What we'll do here |
|---|---|---|
| Build system | CMake (`CMakeLists.txt`) | Same |
| Test framework | GoogleTest, downloaded at configure time via `CMakeLists.txt.in` + `ExternalProject` (pinned to `release-1.10.0`) | Same (copied as-is in step 4) |
| Layout | `code/` (your `.h`/`.cpp`), `app/main.cpp`, `tests/test_*.cpp` | Same |
| Executables | `run_tests` (tests + `code/`), `run_app` (`app/main.cpp` + `code/`) | Same |
| Compiler flags | `-std=c++11 -Wall`, Debug by default | Same |
| Editor | VS Code: `tasks.json` runs `cd build && cmake .. && make`, `launch.json` debugs with `/usr/bin/gdb` | Same files, opened through VS Code's WSL extension |
| Build output | `build/` (git-ignored, with a `.gitkeep` so the folder exists) | Same |

## 1. Install WSL and the tools (one time)

1. In **PowerShell as Administrator**:
   ```powershell
   wsl --install -d Ubuntu
   ```
   Restart if it asks, then open **Ubuntu** from the Start menu and create a Linux username and password.
   (If you already have WSL, `wsl -l -v` lists your distros.)
2. In the Ubuntu terminal, install the compiler, CMake, debugger, and git:
   ```bash
   sudo apt update
   sudo apt install -y build-essential cmake gdb git
   ```
3. Check that everything is there:
   ```bash
   g++ --version
   cmake --version
   gdb --version
   make --version
   ```
   `cmake --version` should be 3.x on current Ubuntu (for example, 3.28 on Ubuntu 24.04).
4. Tell git in WSL not to treat Windows line endings as changes:
   ```bash
   git config --global core.autocrlf input
   ```
   **Why:** Windows git checked the repo out with Windows line endings (CRLF), but WSL's git expects
   Linux line endings (LF). Without this setting, `git status` in WSL shows files you never touched
   (like `README.md`, `.gitignore`, `src/python/a_star.py`) as modified, with every line changed.
   `input` makes git ignore the CRLF/LF difference when comparing, and commit with LF.
   It's a global setting, so it lives in `~/.gitconfig` and you only need to run it once.

   > **Tip:** to look at or edit that file, run `nano ~/.gitconfig`. In nano, save with
   > **Ctrl+O** then **Enter**, and exit with **Ctrl+X**.

## 2. Get to the repo from WSL

Your Windows drive is mounted at `/mnt/c`, so the repo is already there:

```bash
cd /mnt/c/Users/Work/source/repos/q-learning/src
```

> Builds under `/mnt/c` are a little slower than in your Linux home folder (`~`), but for a project
> this size you won't notice. Keeping the repo where it is means Windows tools (gitk, Explorer) still see it.

## 3. Create the folder layout

```bash
cd /mnt/c/Users/Work/source/repos/q-learning/src/cpp
mkdir -p app code tests build .vscode
touch app/main.cpp code/AStar.h code/AStar.cpp tests/test_a_star.cpp CMakeLists.txt .gitignore build/.gitkeep
```

You should end up with:

```
src/cpp/
├── readme-cpp.md          (this file)
├── CMakeLists.txt
├── CMakeLists.txt.in      (downloads GoogleTest)
├── .gitignore
├── .vscode/
│   ├── tasks.json
│   └── launch.json
├── build/
│   └── .gitkeep
├── app/
│   └── main.cpp           (a playground, like the HW repo's main.cpp)
├── code/
│   ├── AStar.h            (your declarations)
│   └── AStar.cpp          (your implementation)
└── tests/
    └── test_a_star.cpp    (GoogleTest tests)
```

File names are up to you, but `CMakeLists.txt` lists every source file by name (step 4), so any file
you add or rename has to be listed there too.

## 4. `.gitignore` and `CMakeLists.txt`

### `.gitignore`

Same as the HW repo. It ignores the build output but keeps the empty `build/` folder:

```gitignore
build/*
!build/.gitkeep
ipch
```

### `CMakeLists.txt` and `CMakeLists.txt.in`

Copy both files from the HW repo, then read through them so you know what each part does:

```bash
cd /mnt/c/Users/Work/source/repos/q-learning/src
cp hw-05/CMakeLists.txt cpp/
cp hw-05/CMakeLists.txt.in cpp/
```

- `CMakeLists.txt.in` downloads **GoogleTest 1.10.0** the first time you configure.
- Line 8 of `CMakeLists.txt` passes `-std=c++11 -Wall` to the compiler for every file. That flag overrides
  whatever your compiler uses by default (for example, g++ 9 on Ubuntu 20.04 defaults to C++14,
  `__cplusplus == 201402L`), and GoogleTest 1.10.0 only needs C++11, so the default doesn't matter.
- With Ubuntu's CMake 3.x you'll see deprecation warnings about `cmake_minimum_required` being below 3.5.
  Those are warnings, not errors.
- **One change from the HW repo:** instead of `file(GLOB ...)`, list the files by name. Replace the two
  `file(GLOB ...)` blocks with:
  ```cmake
  set(USER_FILES
    code/AStar.h
    code/AStar.cpp
  )
  set(TEST_FILES
    tests/test_a_star.cpp
  )
  ```
  When you add a new `.h`/`.cpp` file, add it to the right list. Because `CMakeLists.txt` changed,
  the next `make` re-runs CMake by itself, so you don't need to run `cmake ..` again. (With `GLOB`, a new
  file is silently ignored until you re-run `cmake ..`, which is easy to forget.)

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

## 6. Build and run from the Ubuntu terminal

These are the same commands the HW repo's VS Code task runs:

```bash
cd /mnt/c/Users/Work/source/repos/q-learning/src/cpp/build

cmake ..          # configure (first time, and after adding new files). Downloads GoogleTest the first time.
make              # build both executables

./run_tests       # run the tests
./run_app         # run your playground main.cpp
```

Useful extras:

```bash
./run_tests --gtest_filter=*Unreachable*   # run only matching tests
make -j$(nproc)                            # build faster using all cores
cd .. && rm -rf build/* && cd build        # clean rebuild (keeps build/.gitkeep)
```

The executables are Linux programs, so run them from the Ubuntu terminal, not from PowerShell.

Before writing any A*, a good first check is an empty `main()` plus one trivial test
(`TEST(Sanity, Works) { EXPECT_EQ(1 + 1, 2); }`), so you know the toolchain works before you start
debugging your own code.

## 7. Run the GoogleTest tests

The Python tests are already ported to `tests/test_a_star.cpp`, so there's nothing to write here.
It has the same 11 tests as `src/python/test_a_star.py`:

- **8 grid cases** (`case1_5x5` … `case8_25x25_maze`). These run through one `TEST_P` plus
  `INSTANTIATE_TEST_SUITE_P`, GoogleTest's version of `@pytest.mark.parametrize`. Each case gets
  its own name, like `Cases/AStarGivenCases.test_given_cases/case1_5x5`.
- **3 extra tests**: `test_unreachable_end_returns_empty_list`, `test_straight_line_with_no_walls`,
  and `test_does_not_modify_input_graph`.

Each grid case calls `check_path`, which does the same checks as the Python version: the path starts
at the start cell and ends at the end cell, never steps on a wall, only moves one cell up/down/left/right
at a time, and is as short as the path `bfs_shortest_length` finds.

Build and run them from `src/cpp/build`:

```bash
cd /mnt/c/Users/Work/source/repos/q-learning/src/cpp/build
cmake ..
make
./run_tests
```

**What to expect right now:** `code/AStar.cpp` is a stub that always returns an empty path, so
**3 tests pass and 8 fail**. The 3 that pass are the ones where an empty path is the right answer or
the path doesn't matter: `test_unreachable_end_returns_empty_list`, `case6_20x20_random` (its end
cell can't be reached either), and `test_does_not_modify_input_graph`. As you implement A* in
`AStar.cpp`, watch the failures drop to 0.

Running just some of the tests:

```bash
./run_tests --gtest_filter='Cases/*'              # only the 8 grid cases
./run_tests --gtest_filter='*case1_5x5'           # a single case
./run_tests --gtest_filter='AStar.*'              # only the 3 extra tests
```

When a test fails, read the message under it. `check_path` tells you what was wrong, for example
"path has 12 cells but the shortest path has 10". `./run_app` runs `app/main.cpp`, which prints the
path for the 5x5 grid from case 1. It's handy for checking one grid by eye.

## 8. (Optional) VS Code build and debug

With WSL, the HW repo's `.vscode` files work as they are.

1. In Windows VS Code, install the **WSL** extension (ms-vscode-remote.remote-wsl).
2. From the Ubuntu terminal, open the project folder in VS Code:
   ```bash
   cd /mnt/c/Users/Work/source/repos/q-learning/src/cpp
   code .
   ```
   The bottom-left corner should say **WSL: Ubuntu**. Open `src/cpp` itself, not the repo root,
   so `${workspaceFolder}` points at the right place.
3. When VS Code prompts you, install the **C/C++** extension (ms-vscode.cpptools) *in WSL*.
4. Copy the HW repo's VS Code config:
   ```bash
   cp ../hw-05/.vscode/{tasks.json,launch.json,c_cpp_properties.json} .vscode/
   ```
   `tasks.json` runs `cd build && cmake .. && make`, and `launch.json` has "Run Tests" and "Run App"
   configs that use `/usr/bin/gdb`. That's why step 3 created `build/`.
5. `Ctrl+Shift+B` builds. F5 (after picking "Run Tests" or "Run App" in the Run and Debug panel)
   builds and starts the debugger, so you can set breakpoints in `AStar.cpp`.

## 9. Troubleshooting

| Symptom | Likely cause / fix |
|---|---|
| `Compatibility with CMake < 3.5 has been removed` | You're on CMake 4.x, which rejects the HW repo's old `cmake_minimum_required`. Use Ubuntu's apt CMake (3.x). |
| Deprecation warnings mentioning `cmake_minimum_required` | Normal with the HW repo's files on CMake 3.x; safe to ignore. |
| `code: command not found` in Ubuntu | Install VS Code on Windows with "Add to PATH" checked, then open a new Ubuntu terminal. |
| `git status` in WSL shows every file as modified | Line endings (CRLF vs LF): run `git config --global core.autocrlf input` (step 1.4). If files still show as modified with no content changes, it's file permissions on `/mnt/c`: run `git config core.filemode false` in the repo. |
| `undefined reference to ...` for a function in a new `.cpp` file | The file isn't in `USER_FILES` in `CMakeLists.txt`. Add it there, then run `make`. |
| GoogleTest download fails | Needs internet the first time; after that it's cached in `build/`. |
| `cannot execute binary file` from PowerShell | The build makes Linux programs; run them from the Ubuntu terminal. |

## 10. Checklist

- [ ] WSL Ubuntu installed; `g++`, `cmake`, `gdb`, `make` all print versions
- [ ] `git config --global core.autocrlf input` set in WSL
- [ ] Folder layout created (step 3)
- [ ] `.gitignore`, `CMakeLists.txt` and `CMakeLists.txt.in` in place
- [ ] Sanity test builds and passes
- [ ] `AStar.h` interface written
- [ ] Python tests ported to `tests/test_a_star.cpp`
- [ ] A* implemented and all tests pass
- [ ] (Optional) VS Code over WSL building and debugging
