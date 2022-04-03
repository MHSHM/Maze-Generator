# Maze-Generator
 The program generates a random Perfect Maze Then solves it using one of the well-known algorithms.
 
# Made with
 - C++14
 - OpenGL 3.x
 - GLFW3

# Algorithms
 - Recursive Backtracking for the maze generation.
 - DFS
 - BFS
 - Dijkstra
 - A*

# Techniques
 - Instance Rendering
 - Multi-Threading

# Visualization
 To visualize the maze generation and solving i used the framework i implemented while making my [2D Platformer](https://github.com/MHSHM/2D-Platformer) to help create the grid and render it and also with the help of some multithreading (just for each stage spawn a thread and make it run in the background without any synchronization primitives).
 
 
# Preview
 - https://youtu.be/rr9UcGC40s8
 In the preview the algorithm used for solving the maze was A*. You can change it by going to "Backtracking_Maze_Generator" function and change the function pointer passed to the path finder thread to any other function that implements different algorithm.
 
 # References
  - https://weblog.jamisbuck.org/2010/12/27/maze-generation-recursive-backtracking
  - https://weblog.jamisbuck.org/2015/10/31/mazes-blockwise-geometry.html
  - https://learnopengl.com/Advanced-OpenGL/Instancing
  - https://www.youtube.com/watch?v=Y37-gB83HKE 
