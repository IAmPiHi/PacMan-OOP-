# Pac-Man: OOP Semester Project

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![Library](https://img.shields.io/badge/library-SFML-green.svg)
![Author](https://img.shields.io/badge/Author-Chris-orange.svg)

> **Semester Project for Object-Oriented Programming (OOP)**
>
> A recreation of the classic arcade game Pac-Man, developed using C++ and the SFML library. This project focuses on implementing robust OOP architecture, autonomous AI pathfinding, and procedural 2D graphics.

![image](https://lh3.googleusercontent.com/rd-d/ALs6j_HjzeIiUwYAPXQ7W8O82ZtVWV5vhG-Gr488YmRIko21-BQjidylinyS-qHfNrcy_oaaSrNINhoVpARKXt4T02iOhzZIAdFatvOT76BKq5Nl-vHudw4SSrZmJ9cZPnAGOvqbhuftjjO0MXvbu9a8SzQUa6Kd4p0xe6E52NTR2WNxoKFiF9ltfoD6Lw4hnz6XfbBWHZSExXnkNa9sBO9pwZUSuF9TcDOUHR74RTmtnBAylPlmGTRygSRF_dDXLY8U8z7oUM4Qv9EK140XtYVtBn28txftgFUrMz1PlyI6P9Iv8FOq5sAh3S8KyyO_cpILlThPV7RljXoGCP2JzFxt145gomS_S49VpZ4OlhlJ3dd1NT0OVD2hwm9m59Cp2X0nnUx7WM-zkEDzXvb3RJcLSPb3NmX1Tr8Ko9LHB8XAFSpjH4UcAZKDqFuqRKdYqA4OsswNqrWCFpfil6rCgWXX6VOt8_JZRmZKoJxtzkZdwzgqoQrczporZKe3sxm0FjI3kI0gj4R6D784tKBAUfA2WqDmDlRpUJMvBkPqno5O8h429GNjl_hLIPnBYt078-L6nhaOdNsu19YlQz6K52SiZjMcYY4F2GbdztooK05F_eW9KPisbwLmG5NaZ5uBYWRAUO5wLGsii-PQQEG609JlP8q7A7JPPUH5Gz_AywlN42pXzF_BiWf7KP5vmP2ni0nan9C5-khxd7JKPfEcz2vUmV3K8d9lmF4cr7yyiOSWVX5SU8TqLNXeawp4MqwYCYpSujJrOa7eRCtdERt1NPo0eJmkQI0tcd0stub0qXkXfp0YDbjJ9PMEttGqn1-t5pbnPls9IJn-ipDnD2MEBRo24evRhqN0_cuiAAvhpMPtJhm_fAl7m6JRRdXwHvYKk1FLswLggfChulyT9fQy0f2Fipxtc1owyZrdwjGAKfvISG-SSk6OkrTguu9tLAgok4Cne45SSbG2VsEVsyTUK1wrfVZ0L0BztS_S-m_l_6ulzM-Y69IQ-EVigBdBMz1OZPFiYu_ajx2waT3dFQH4iMZ9o_jy5SRKf_vtjdNOhyB8z_RujymcRs87_9Jac3hM_pzsM2qZsvRmTfAoM5W1snsOBsPL6UWy1_dticJv8zVP5LiIPV11vefbPS9qv7HlK1NUG3ogRjZkXrUGaM8XPOkW0d9REOt46H8gYTcsS7L68v0Rag58EH_DZ9a2C1U8OTkAQkFRdwxcIbjds2sz300nVYxz2OIcuZ-99faIZz5iEpfki-UJSRoxQ25BQXXCBwGfq3VLIGvYJRzvbsMnPleA4Qqxkd2ro8OdZOxsiKY4X6D1ECMWJuOeO7eQ6Wd05ny9QeqYPtYzcQ=w2549-h1274?auditContext=prefetch)
## 📖 Introduction

This project demonstrates the application of core Computer Science concepts within a real-time game engine environment. Unlike standard sprite-based games, this implementation relies on **mathematical geometry** for rendering and **graph theory** for enemy artificial intelligence.

## Game Rules

1.  **Evade Ghosts:** Avoid being caught by the pursuing ghosts.
2.  **Power Up:** A magic star appears randomly every **12 seconds**.
3.  **Turn the Tables:** Collect the star to become invincible and hunt down the ghosts!
4.  **Watch Out:** If a ghost eats the star first, you will be **frozen for 3 seconds**!

## ✨ Key Features

* **Smart Ghost AI:** Ghosts are not random; they utilize the **A* (A-Star) Pathfinding Algorithm** to calculate the shortest route to the player in real-time.
* **Procedural Animation:** Pac-Man is rendered using dynamic `sf::VertexArray` geometry. The mouth animation is calculated using continuous angular interpolation (0° to 40°), creating a smooth "breathing" motion independent of movement speed.
* **Grid-Based Collision:** Precise collision detection system ensuring valid movement within the maze layout.
* **Classic Gameplay Loop:** Includes pellet collection, score tracking, and win/loss states.

## 🏗️ Technical Implementation

### 1. Object-Oriented Design (OOP)
The project adheres to OOP principles to ensure maintainability and scalability:
* **Encapsulation:** Game entities (`PacMan`, `Ghost`) manage their own state (position, direction, animation timer) and expose only necessary methods to the main game loop.
* **Modularity:** Rendering logic is decoupled from game logic. The map is managed as a distinct data structure, allowing for easy level modification.

### 2. A* Pathfinding Algorithm
The Ghost agents use the A* algorithm to pursue the player. The algorithm selects the path that minimizes:
$$f(n) = g(n) + h(n)$$
* **g(n):** The cost from the start node to the current node.
* **h(n):** The heuristic estimated cost to the target (Pac-Man).
* **Heuristic:** We utilize **Manhattan Distance** ($|x_1 - x_2| + |y_1 - y_2|$) as movement is restricted to 4 cardinal directions.

### 3. Smooth Geometry Rendering
Instead of pre-loaded textures, the Pac-Man character is drawn using a Triangle Fan primitive. The mouth angle is updated every frame via a floating-point counter, allowing for fluid, high-framerate animation that looks superior to traditional sprite-sheet swapping.

## 🛠️ Build & Run

### Prerequisites
* C++ Compiler (supporting C++17 standard)
* SFML Library (version 2.5.1 or higher)
* And modify makefile.txt make sure it has the right info

### Compilation Example (G++)
```bash
g++ -c main.cpp PacMan.cpp Ghost.cpp
g++ main.o PacMan.o Ghost.o -o pacman-game -lsfml-graphics -lsfml-window -lsfml-system
```
or you can also use 
```bash
make
```
That makefile is for Windows so if you're using other OS you have to rewrite it.<br>
Also even if you're using Windows you still have to modify the SFML path.

