# Multi-Threaded PACMAN in GLUT with Synchronization

Welcome to Multi-Threaded PAC-MAN in GLUT with Synchronization! This project aims to develop a multi-threaded version of the classic Pac-Man game using C/C++ and GLUT (OpenGL Utility Toolkit) graphics library, incorporating synchronization techniques to manage interactions between Pac-Man and multiple computer-simulated ghosts. The game ensures smooth gameplay with each entity operating on its own thread and proper synchronization to prevent data conflicts.

## Project Objective

The objective of this project is to implement a multi-threaded Pac-Man game with the following phases:

### Phase 1 - Setting up Dedicated Threads for Game Modules:
- **Game Engine Thread**: Manages overall game flow, input handling, game state updating, and graphics rendering.
- **User Interface Thread**: Manages UI components including menus, scoreboards, and HUD elements, and communicates player input to the game engine.
- **Ghost Controller Threads**: Individual threads for each ghost controller, responsible for controlling ghost behavior independently.

### Phase 2 - Basic Game Mechanics:
- **Game Board Initialization**: Design layout for shared game board, including walls, paths, pellets, and power pellets. Initialize starting positions for Pac-Man and ghosts.
- **Movement Mechanics**: Implement movement logic for Pac-Man and basic AI for ghost movement.
- **Eating Mechanics**: Enable Pac-Man to eat pellets, update game score, and define effects of eating power pellets.
- **Lives**: Implement a lives system for Pac-Man.

### Phase 3 - Synchronization:
- **Ghost Movement**: Ensure synchronization to prevent conflicts when ghosts read the game board while Pac-Man updates it.
- **Eating Power Pellets**: Synchronize power pellet eating to prevent simultaneous consumption and ensure respawn consistency.
- **Ghost House**: Manage resources (keys and exit permits) to prevent deadlock situations when ghosts leave the ghost house.
- **Prioritizing Certain Ghosts**: Implement synchronization to ensure fair distribution of speed boosts among faster ghosts.

## Implementation

- **Separate Threads**: Pac-Man and each ghost operate on separate threads for independent movement.
- **Proper Synchronization**: Use mutexes to ensure proper synchronization and prevent data conflicts.
- **Concurrent Execution**: Implement concurrent execution allowing smooth movement of Pac-Man and ghosts without lag or jitter.
- **Optimal Synchronization Solutions**: Choose appropriate synchronization solutions for each scenario to ensure optimal game performance and fairness.

## Instructions

- **Compilation**: Compile the code using a C compiler with proper linking to GLUT library.
- **Execution**: Run the compiled executable to start the game.
- **Controls**: Use arrow keys to control Pac-Man's movement.
- **Gameplay**: Eat pellets, avoid ghosts, and complete levels to win.

## Contributions

Contributions to enhance the game mechanics, optimize synchronization, or improve overall gameplay experience are welcome! Please follow project guidelines and submit pull requests with your changes.
