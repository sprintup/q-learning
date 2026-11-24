## Introduction
I’d like to focus on implementing a reinforcement learning algorithm for my final project. Specifically, something like a gridworld maze. I have seen this demoed before and appreciated it then, but want to get my hands dirty implementing it. I brought up q-learning in my proposal and it was recommended that I look into A*. I subscribed to algo-expert.io which has an explainer on A* and I hope to learn that algorithm and any surrounding algorithms in C++ (A language I'm newer to), in part as preparation to an algorithms course I plan to take next term. 

Ultimately, I am still most interested in implementing a reinforcement learning algorithm, even if it's a ambitious goal. I've completed a Applied Data Science certificate from MIT-PE, where I made an image classification model as a convolutional neural network, before, so I'm not starting from square one. The space of Ai alignment is something that interests me. 

Since I'm leveraging Ai in this project, I have to set a more challenging stretch goal to make it suitable as my final project. I intend to strike a balance between leveraging Ai as a tool for learning and not cognitively offloading the work. One project that I recently became familiar with is https://rubicon-eval.github.io/ where models are tested for whether the recognize they've been given an impossible task or if they know when they pass a 'point of no return.' I also listened to this article [How to read the AI mind](https://www.economist.com/podcasts/2026/09/23/how-to-read-the-ai-mind) a while back, which digs into whether Ai have "emotions" more or less and also this youtube video [How Stanford Teaches AI Coding Now](https://youtu.be/0_JtfuXVI10?si=S9EcOC1EEslsMvV_&t=1373) about traces. I'd like to aim to bring this all together in a way where I perhaps am able to trace the activations of an open weight local model in some way as it makes its way through a impossible gridworld maze to see if I can recognize any 'emotions' like doom when it passes the 'point of no return' or recognizes a puzzle is impossible. This is a pretty ambitious target, but I might learn some interesting things about CUDA++, which is something I've wanted to try. 

## Housekeeping
- This README.md file was handwritten (not generated or modified by Ai). The other .md files in this project are generated and serve as resources I can reference as I work through my project. 
- Since there are multiple things I'm trying to get done during this project, I am going to use this file as a 'checklist' of sorts that explains what has happened in each commit. I'll add items to the todo list and as I accomplish them, I'll check them off the todo list and commit them. 
- The final version of this project is likely to be more than what is required, so it might be beneficial to look at the project commit by commit. If you're on windows, you can use a `gitk --all` to see all the commits in a visualizer. At each step, I will add a new section to this readme explaining what I've done. 
- To make this less confusing (hopefully) I split the different 'Feature Todo' list items below into different branches and I plan to submit a PR against master for each one. This will hopefully help manage the clutter of working with AI generated clutter from the actual  each todo item will have several commits. So you should be able to look at the PR to see all the changes for a given todo list item. I also numbered them to make it easier. Each todo list item will have a separate section below, with a sub checklist that I will handle in commits. 

## Feature Todo List
- 1. [ ] Implement a working version of A*
- 2. [ ] Implement a working version of q-learning
- 3. [ ] Leverage CUDA++ for q-learning
- 4. [ ] Research how to fabricate a visualization for activations of model
- 5. [ ] Build a step by step state logger that logs the state of each step, including the visual representation of the 'emotion'
- 6. [ ] Build an impossible puzzle and send the model through it
- 7. [ ] Analyze the results to see if any step might show an emotion like 'doom' when the model passes the point of no return or recognizes the model is impossible. 

# Notes
## Implement a working version of A*
After watching the tutorial of A* Algorithm, it seems pretty straight forward. However, it was also mentioning how it differs from Dijkstra's Algorithm. Since it brought up that algorithm, I thought I'd watch that video as well. It seems the heuristic used in A* expands directly toward the end location, whereas Dijstra's extends in every direction evenly. The tutorial walkthrough was in python. One thing that was interesting was how the instructor initialized the scores to infinity, I'll have to figure out how to do that. I did appreciate that I was able to follow the tutorial better than before. I had an Algo Expert membership before, but often didn't really follow the solutions that well, but after taking data structures, I'm following a lot easier. 
- [x] Review Piazza

### A* breakdown
I will now explain how to implement the algorithm. Basically, there is a m x n grid full of obstacles, a start and end. The algorithm assigns each cell 3 scores, H, G, F. H is the distance from the start, G is the distance to the end (calculated using a heuristic like the Manhattan Coefficient). F is just the addition of H and G. The algorithm assigns an F score to each eligible cell in the grid and proceeds along the optimal path (lowest number), so long as the H score for a given cell doesn't increase.  The algorithm itself puts the cells in a priority queue implemented as a min heap. At each step, the algorithm will also assign to the cell an arrow for the direction of the last cell visited, so that when execution reaches the end of the grid, it can build the optimal paths by following the arrows back to the start.
- [x] Watch A* video
- [x] Explain the algorithm in this project

## A* Complexity
During the video on A*, it discussed how to time complexity would be O(N) if N is w x h (the number of nodes in the graph). Then find the node with the smallest f score. Instead of that we should use a priority queue implemented as a min heap, which should take time complexity  O(Nlog(N)) where N = w * h. 

## Implement the A* algorithm
One of the components of the algorithm is implementing a min heap, which we did in C++ in class, but which I got some questions wrong on the final so I wanted to dig into it a bit more. I started by looking at the algorithm in Algo expert, but then realized that for A* I can probably just use the priority_queue within the std library in C++. Nevertheless, I reimplemented min heap in python along with A* and my plan is to move it into C++. 

The parts I found interesting is identifying the surrounding nodes and adding them to the neighbors array. 

- [x] Implement the A* algorithm in python
- [x] Watch Priority queue as min heap video
- [x] Provision python A* with tests

### Implement A* in C++
I cloned the priority queue repo into the project to get an idea for how to structure the C++ implementation. I then had Ai generate a readme describing step by step how to set up the structure. 

The setup was pretty straight forward, however, since I am newer to C++, the CMakeLists.txt file and CMakeLists.txt.in file were new to me so I stopped to read them over and learn a bit about what they do. 

I had Ai port over the tests so I had something to work toward when implementing my version of A*. The test code is always kind of a nightmare to get to work and more sophisticated than I'm ready for at the moment. I figure this is a good compromise. At least this way I can do some test driven development. 

- [ ] Implement the A* in C++
  - [x] Look at homework assignments for min heap and project setup
    - [x] clone it into project to get structure

I got a little side tracked here learning about submodules. I added src\hw-05 as a submodule and instructions for how to restore it (if you want/need) are located in notes\submodules.md

  - [x] 'min heap construction'
- [ ] Read missed sections on heaps sections on 
- [ ] Watch Dijstra's algorithm video
