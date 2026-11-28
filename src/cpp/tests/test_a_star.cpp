// Tests for code/AStar.cpp, ported from src/python/test_a_star.py.
//
// Each case checks that the path returned by aStarAlgorithm:
//   * starts at the start cell and ends at the end cell,
//   * only moves one step up/down/left/right at a time,
//   * never steps on a wall (1),
//   * is a shortest path (same length as a simple breadth-first search finds).
// If the end can't be reached, the expected result is an empty list.
//
// Build and run from src/cpp/build:
//   cmake .. && make && ./run_tests

#include <gtest/gtest.h>
#include "test_a_star.h"

#include <cstdlib>
#include <ostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

using namespace std;



// ---------------------------------------------------------------------------
// Reference shortest path length (plain breadth-first search)
// ---------------------------------------------------------------------------

// Number of cells on the shortest path (start and end included), or -1 if the
// end can't be reached. (-1 plays the role of Python's None.)
int bfs_shortest_length(const Grid& graph, int startRow, int startCol,
                        int endRow, int endCol) {
  int rows = graph.size();
  int cols = graph[0].size();

  // Each queue entry is ((row, col), length so far).
  queue<pair<pair<int, int>, int>> q;
  vector<vector<bool>> seen(rows, vector<bool>(cols, false));

  q.push(make_pair(make_pair(startRow, startCol), 1));
  seen[startRow][startCol] = true;

  const int dr[] = {1, -1, 0, 0};
  const int dc[] = {0, 0, 1, -1};

  while (!q.empty()) {
    int r = q.front().first.first;
    int c = q.front().first.second;
    int length = q.front().second;
    q.pop();

    if (r == endRow && c == endCol) {
      return length;
    }
    for (int i = 0; i < 4; i++) {
      int nr = r + dr[i];
      int nc = c + dc[i];
      if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
          graph[nr][nc] == 0 && !seen[nr][nc]) {
        seen[nr][nc] = true;
        q.push(make_pair(make_pair(nr, nc), length + 1));
      }
    }
  }
  return -1;
}

void check_path(const Case& c, const Path& path) {
  int expected_length =
      bfs_shortest_length(c.grid, c.startRow, c.startCol, c.endRow, c.endCol);

  if (expected_length == -1) {
    EXPECT_TRUE(path.empty()) << "end is unreachable, expected [] but got "
                              << path.size() << " cells";
    return;
  }

  // ASSERT_ (not EXPECT_) because path[0] below would crash on an empty path.
  ASSERT_FALSE(path.empty()) << "expected a path but got an empty list";

  // Every entry must be a {row, col} pair before we index into it.
  for (size_t i = 0; i < path.size(); i++) {
    ASSERT_EQ(path[i].size(), 2u) << "path[" << i << "] is not a {row, col} pair";
  }

  EXPECT_EQ(path.front(), (vector<int>{c.startRow, c.startCol}))
      << "path does not start at the start cell";
  EXPECT_EQ(path.back(), (vector<int>{c.endRow, c.endCol}))
      << "path does not end at the end cell";

  for (size_t i = 0; i < path.size(); i++) {
    int r = path[i][0];
    int col = path[i][1];
    ASSERT_TRUE(r >= 0 && r < (int)c.grid.size() && col >= 0 &&
                col < (int)c.grid[0].size())
        << "path leaves the grid at [" << r << ", " << col << "]";
    EXPECT_EQ(c.grid[r][col], 0)
        << "path goes through a wall at [" << r << ", " << col << "]";
  }

  for (size_t i = 0; i + 1 < path.size(); i++) {
    int r1 = path[i][0], c1 = path[i][1];
    int r2 = path[i + 1][0], c2 = path[i + 1][1];
    EXPECT_EQ(abs(r1 - r2) + abs(c1 - c2), 1)
        << "step from [" << r1 << ", " << c1 << "] to [" << r2 << ", " << c2
        << "] is not a single up/down/left/right move";
  }

  EXPECT_EQ((int)path.size(), expected_length)
      << "path has " << path.size() << " cells but the shortest path has "
      << expected_length;
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

// TEST_P + INSTANTIATE_TEST_SUITE_P is GoogleTest's version of
// @pytest.mark.parametrize: test_given_cases runs once for every entry in
// CASES, and each run is named after the case (for example
// Cases/AStarGivenCases.test_given_cases/case1_5x5).
class AStarGivenCases : public ::testing::TestWithParam<Case> {};

TEST_P(AStarGivenCases, test_given_cases) {
  const Case& c = GetParam();
  Path path = aStarAlgorithm(c.startRow, c.startCol, c.endRow, c.endCol, c.grid);
  check_path(c, path);
}

// Names each run after its case instead of 0, 1, 2, ...
string CaseName(const ::testing::TestParamInfo<Case>& info) {
  return info.param.name;
}

INSTANTIATE_TEST_SUITE_P(Cases, AStarGivenCases, ::testing::ValuesIn(CASES),
                         CaseName);

TEST(AStar, test_unreachable_end_returns_empty_list) {
  // The end cell is boxed in by walls.
  Grid graph = {
      {0, 0, 0},
      {0, 1, 1},
      {0, 1, 0},
  };
  EXPECT_EQ(aStarAlgorithm(0, 0, 2, 2, graph), Path{});
}

TEST(AStar, test_straight_line_with_no_walls) {
  Grid graph = {{0, 0, 0, 0, 0}};
  Path expected = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}};
  EXPECT_EQ(aStarAlgorithm(0, 0, 0, 4, graph), expected);
}

TEST(AStar, test_does_not_modify_input_graph) {
  // aStarAlgorithm takes the graph by const reference, so the compiler
  // already stops it from changing the grid. This test still checks it, the
  // same way the Python test does.
  const Case& c = CASES[0];
  Grid graph = c.grid;
  Grid original = graph;
  aStarAlgorithm(c.startRow, c.startCol, c.endRow, c.endCol, graph);
  EXPECT_EQ(graph, original);
}
