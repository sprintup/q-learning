// YOUR tests for A* go in this file.
//
// Convention so your tests stand out in the terminal:
//   * Your suite is named MY_AStar.
//     GoogleTest prints the suite name on every line, so you will see
//         [ RUN      ] MY_AStar.start_equals_end
//     right next to the given tests (AStar.*, AStarGivenCases.*,
//     test_PriorityQueue.*).
//   * Use TEST_F with the MY_AStar fixture below (or type ut + Tab).
//     It prints a yellow banner once before its tests run.
//
// Run only your tests (from src/cpp/build):
//   ./run_tests --gtest_filter='MY_*'
// Run everything except yours:
//   ./run_tests --gtest_filter='-MY_*'

#include "../code/AStar.h"
#include <gtest/gtest.h>

#include <iostream>
#include <vector>
#include "test_a_star.h"

using namespace std;

namespace {  // keeps these names private to this file

void printBanner(const char* title) {
  // "\033[1;33m" = bold yellow, "\033[0m" = back to normal.
  cout << "\033[1;33m"
       << "==================== " << title << " ===================="
       << "\033[0m" << endl;
}

}  // namespace

// ---------------------------------------------------------------------------
// Fixture. You don't need to change this.
// ---------------------------------------------------------------------------

class MY_AStar : public ::testing::Test {
 protected:
  static void SetUpTestSuite() { printBanner("MY A* TESTS"); }
};

// ---------------------------------------------------------------------------
// Example (commented out so it doesn't fail while aStarAlgorithm is a stub).
// Uncomment it, or copy its shape for your own tests.
// ---------------------------------------------------------------------------

// TEST_F(MY_AStar, start_equals_end) {
//   vector<vector<int>> graph = {
//       {0, 0},
//       {0, 0},
//   };
//   vector<vector<int>> path = aStarAlgorithm(1, 1, 1, 1, graph);
//
//   vector<vector<int>> expected = {{1, 1}};
//   EXPECT_EQ(expected, path);
// }

// ---------------------------------------------------------------------------
// Your A* tests
// ---------------------------------------------------------------------------

// initializeNodes: when aVectorGraph, expect aNodeGraph.
//
// RED:   does not return a node graph that matches the vector graph
// GREEN: returns a node graph that matches the vector graph
TEST_F(MY_AStar, initializeNodes_aVectorGraph_aNodeGraph) {
  // arrange
  Grid grid = CASES.at(0).grid;
  // act
  Graph graph = initializeNodes(grid);
  
  // assert - make it fail for the right reason first
  ASSERT_EQ(grid.size(), graph.size());// rows (ai receommended)
  ASSERT_EQ(grid[0].size(),graph[0].size());// cols (ai receommended)
}