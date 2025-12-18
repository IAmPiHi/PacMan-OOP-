# Pac-Man: OOP Semester Project

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![Library](https://img.shields.io/badge/library-SFML-green.svg)
![Author](https://img.shields.io/badge/Author-Chris-orange.svg)

> **Semester Project for Object-Oriented Programming (OOP)**
>
> A recreation of the classic arcade game Pac-Man, developed using C++ and the SFML library. This project focuses on implementing robust OOP architecture, autonomous AI pathfinding, and procedural 2D graphics.

## 📖 Introduction

This project demonstrates the application of core Computer Science concepts within a real-time game engine environment. Unlike standard sprite-based games, this implementation relies on **mathematical geometry** for rendering and **graph theory** for enemy artificial intelligence.

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

