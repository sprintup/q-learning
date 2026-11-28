#ifndef ASTAR_H__
#define ASTAR_H__

#include <vector>
#include <string>
#include <limits>

using namespace std;

struct Node {
  string id;
  int row;
  int col;
  int value; 
  int distanceFromStart = std::numeric_limits<int>::max();
  int estimatedDistanceToEnd = std::numeric_limits<int>::max();;
  Node* cameFrom = nullptr;
  
  Node(int row, int col, int value)
    : id(std::to_string(row) + "-" + std::to_string(col)), row(row), col(col), value(value) {};
};

typedef vector<vector<int>> Grid;  // grid[row][col], 0 = open, 1 = wall
typedef vector<vector<Node>> Graph;  // graph[row][col], 0 = open, 1 = wall
typedef vector<vector<int>> Path;  // list of {row, col}

// aStarAlgorithm finds a shortest path on a grid, just like the Python
// version in src/python/a_star.py.
//
//   grid   : a grid of 0 (open cell) and 1 (wall) [row][col]. 
//   graph  : a node graph with a grid.
//   start   : (startRow, startCol)
//   end     : (endRow, endCol)
//   moves   : one step up, down, left, or right (no diagonals).
//
// Returns the path as a list of {row, col} pairs, from the start cell to the
// end cell (both included). If the end can't be reached, returns an empty
// list.
//
// graph is passed by const reference: the function can read it but can't
// change it (that is what the "does not modify input graph" test checks).
Path aStarAlgorithm(int startRow, int startCol, int endRow, int endCol, const Grid& grid);

// Add your Node struct, min-heap, and helper function declarations here
// (for example: calculateManhattanDistance, getNeighboringNodes,
// reconstructPath), and implement them in AStar.cpp.

Graph initializeNodes(Grid grid);

#endif // ASTAR_H__
