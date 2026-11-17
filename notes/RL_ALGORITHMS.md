> AI-generated study note — not part of the graded write-up.

# Reinforcement learning algorithms

## How many "types" are there?

There's no fixed count. RL algorithms are usually sorted along a few
independent axes, and any one algorithm sits somewhere on each:

| Axis | Options | Q-learning is… |
|---|---|---|
| Does the agent have/learn a model of the environment? | **Model-free** vs **model-based** | Model-free |
| What does it learn? | **Value-based** (how good is each state/action), **policy-based** (learn the action choice directly), or **actor-critic** (both) | Value-based |
| Which policy does it learn about? | **On-policy** (the one it's following) vs **off-policy** (a different, usually greedy, one) | Off-policy |
| How are values stored? | **Tabular** (a table/hash map) vs **function approximation** (e.g. a neural network) | Tabular |
| How does it learn from experience? | **Monte Carlo** (wait for episode end), **temporal-difference / TD** (update every step), or **dynamic programming** (needs the full model) | TD |

## The algorithms

✅ = realistic for a C++ gridworld final project without a neural-network library.

### Foundations (need the full model — "planning")

| Algorithm | Notes | Fit |
|---|---|---|
| Policy Iteration | Alternate "evaluate policy" and "improve policy" until stable. | ✅ |
| Value Iteration | Repeated Bellman updates over every state. Closest cousin of A*/Bellman-Ford. | ✅ |

### Multi-armed bandits (RL with a single state)

| Algorithm | Notes | Fit |
|---|---|---|
| ε-greedy | Explore at random with probability ε, otherwise exploit. Q-learning uses this to pick actions. | ✅ |
| UCB (Upper Confidence Bound) | Prefer actions you're uncertain about. | ✅ |
| Thompson Sampling | Bayesian exploration. | ✅ |

### Tabular, model-free (learn by trial and error)

| Algorithm | Notes | Fit |
|---|---|---|
| Monte Carlo Control | Update values only after an episode finishes. | ✅ |
| SARSA | On-policy TD. Like Q-learning but uses the action actually taken next; tends to learn "safer" paths. | ✅ |
| **Q-learning** | Off-policy TD. Uses `max` over next actions. Your planned core algorithm. | ✅ |
| Expected SARSA | Averages over next actions instead of `max` or sampling. | ✅ |
| Double Q-learning | Two Q-tables to fix Q-learning's tendency to overestimate. | ✅ |
| n-step TD / n-step SARSA | Look several steps ahead before updating. | ✅ |
| TD(λ), SARSA(λ) with eligibility traces | Spread credit back along the recent path — speeds learning in long mazes. | ✅ |

### Tabular, model-based (learn a model, then plan with it)

| Algorithm | Notes | Fit |
|---|---|---|
| Dyna-Q | Q-learning plus "imagined" replays from a learned model. A nice blend of planning (A*-style thinking) and learning. | ✅ |
| Prioritized Sweeping | Dyna-Q that updates the most-changed states first using a **priority queue** — same structure A* uses. | ✅ |
| Monte Carlo Tree Search (MCTS) | Builds a search tree by simulated playouts (used in AlphaGo). | ✅ (harder) |

### Deep RL — value-based (neural network replaces the Q-table)

| Algorithm | Notes |
|---|---|
| DQN (Deep Q-Network) | Q-learning with a neural net, replay buffer, target network. |
| Double DQN, Dueling DQN | Fixes/extensions to DQN. |
| Prioritized Experience Replay | Replay buffer ordered by a priority queue. |
| Rainbow | Combines six DQN improvements. |

### Deep RL — policy gradient & actor-critic

| Algorithm | Notes |
|---|---|
| REINFORCE | Simplest policy gradient. Could be done tabular in a gridworld. |
| A2C / A3C | Advantage actor-critic (synchronous / asynchronous). |
| TRPO | Trust-region policy updates. |
| PPO | Clipped, stable policy updates; the default workhorse today. |
| DDPG, TD3 | Actor-critic for continuous actions. |
| SAC (Soft Actor-Critic) | Off-policy actor-critic with an entropy bonus. |

### Deep RL — model-based and game playing

| Algorithm | Notes |
|---|---|
| AlphaZero | MCTS + neural network, self-play. |
| MuZero | Learns its own model, then plans with MCTS. |
| Dreamer | Learns a world model and trains inside it. |

### RL for language models

| Algorithm | Notes |
|---|---|
| RLHF (with PPO) | Reward model trained from human preferences. |
| DPO | Preference learning without an explicit RL loop. |
| GRPO | Group-relative policy optimization, used for reasoning models. |

The NYT article in your proposal is about **reward hacking** — an agent finding
an unintended way to collect reward. A gridworld can show this directly: give
a small reward for picking up the key and see whether the agent learns to keep
re-triggering it instead of reaching the door.

## Ideas for the project, from simplest to most ambitious

1. **Q-learning only** on a 5x5 key-and-door grid; Q-table as `std::unordered_map`
   keyed by `(row, col, hasKey)`.
2. **Q-learning vs SARSA** on the same maze (add a "cliff" or lava cells) and
   compare the paths they learn.
3. **Value Iteration vs Q-learning** — one knows the map, one doesn't; compare
   how many updates each needs.
4. **Dyna-Q or Prioritized Sweeping** — adds a priority queue, so the project
   showcases two data structures.
