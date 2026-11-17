> AI-generated study note — not part of the graded write-up.

# AlgoExpert questions that build toward A* (and toward Q-learning)

Question names are from AlgoExpert's catalog as best recalled; if one is named
slightly differently on the site, search for the key word.

## How A* relates to Q-learning (short version)

Both try to get an agent from a start to a goal by the best route, but they
assume very different things:

| | A* | Q-learning |
|---|---|---|
| Knows the map/rules ahead of time? | Yes (needs the graph and edge costs) | No — learns by trial and error |
| Output | One best path, computed once | A table of values `Q(state, action)` that becomes a policy for every state |
| Guidance | A heuristic `h(n)` (e.g. Manhattan distance) | Rewards collected while exploring |
| Core update | `f(n) = g(n) + h(n)`, pop lowest `f` from a priority queue | `Q(s,a) ← Q(s,a) + α[r + γ·max Q(s',a') − Q(s,a)]` |
| Data structures | Min-heap + hash set/map | Hash map (or 2D array) keyed by state |

So A* is useful background, but it is a **planning** algorithm. The stronger
conceptual bridge to Q-learning is **dynamic programming on a graph** (Bellman-Ford,
grid DP), because the Q-learning update is a sampled version of the Bellman
equation. The list below is ordered with that in mind.

## Tier 1 — Grid & graph traversal (the gridworld itself)

| Question | Why it helps |
|---|---|
| Breadth-first Search | BFS on an unweighted grid already finds shortest paths; A* is "BFS with priorities". |
| Depth-first Search | Basic graph traversal and visited-set bookkeeping. |
| River Sizes | Treating a 2D matrix as a graph — exactly how a gridworld is stored. |
| Remove Islands | More grid-as-graph practice, boundary handling. |
| Minimum Passes Of Matrix | Multi-source BFS in "waves" on a grid; feels like values spreading outward. |
| Knight Connection | BFS on an implicit grid where the moves define the edges (like an agent's action set). |
| Cycle In Graph | Detecting loops — agents in gridworlds love walking in circles. |

## Tier 2 — Priority queues & weighted shortest paths (the A* machinery)

| Question | Why it helps |
|---|---|
| Min Heap Construction | A* and Dijkstra run on a min-heap; build one yourself in C++. |
| Dijkstra's Algorithm | A* is Dijkstra plus a heuristic. Do this right before or after the A* module. |
| A* Algorithm | The module you're watching. Try Manhattan distance on a grid with walls. |
| Laptop Rentals | Extra min-heap practice in a different setting. |

## Tier 3 — Bellman-style dynamic programming (the bridge to Q-learning)

| Question | Why it helps |
|---|---|
| Number Of Ways To Traverse Graph | DP over a grid where each cell's value depends on its neighbors — the same shape as value iteration. |
| Detect Arbitrage | Uses Bellman-Ford. Repeated "relax every edge" updates are the ancestor of the Bellman equation Q-learning is built on. |
| Staircase Traversal | Small DP that shows building a value from future steps. |
| Min Number Of Jumps | Choosing the best action from each position — a mini "policy". |
| Water Area | DP where each cell needs information from both sides; good grid intuition. |

## Tier 4 — Hashing state (the Q-table)

Your proposal mentions hashing game state. In a key-and-door gridworld the state
is `(row, col, hasKey)`, so the Q-table is a hash map from that tuple to four
action values.

| Question | Why it helps |
|---|---|
| Group Anagrams | Designing a hash key from a structured value. |
| LRU Cache | Hash map + linked list working together in a custom C++ class. |
| Boggle Board | Grid traversal combined with a lookup structure (trie) and visited state. |

## Suggested order

BFS → River Sizes → Min Heap Construction → Dijkstra's Algorithm → **A\*** →
Number Of Ways To Traverse Graph → Detect Arbitrage → Group Anagrams → start the
Q-table in C++.
