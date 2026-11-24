#include <iostream>
#include <vector>

#include "../code/AStar.h"

using namespace std;

// A playground, like the HW repo's main.cpp. Try your A* on small grids here
// and print what comes back.
int main() {
  vector<vector<int>> graph = {
      {0, 0, 0, 0, 0},
      {0, 1, 1, 1, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 1, 1, 1},
      {0, 0, 0, 0, 0},
  };

  vector<vector<int>> path = aStarAlgorithm(0, 1, 4, 3, graph);

  cout << "Path has " << path.size() << " cells:" << endl;
  for (size_t i = 0; i < path.size(); i++) {
    cout << "  [" << path[i][0] << ", " << path[i][1] << "]" << endl;
  }

  return 0;
}
