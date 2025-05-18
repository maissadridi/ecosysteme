# Ecosystem Simulation

A C++/SFML simulation of a predator-prey ecosystem with wolves, sheep, grass, and minerals.

![Ecosystem Simulation Screenshot](screenshot.png)

## Features

- 🐑 Sheep that eat grass
- 🐺 Wolves that hunt sheep
- 🌱 Grass that regrows with minerals
- ⚖️ Balanced ecosystem dynamics
- 🎨 Visual interface with SFML
- 📊 Real-time statistics tracking

## Requirements

- C++14 or newer
- SFML library
- Git (for version control)

## Installation

### Linux (WSL/Ubuntu)

1. Install dependencies:
```
sudo apt-get install g++ libsfml-dev
```
Clone the repository:

```
git clone https://github.com/yourusername/ecosystem-simulation.git
cd ecosystem-simulation
```
Compile:

```
g++ -std=c++14 main.cpp -o ecosystem -lsfml-graphics -lsfml-window -lsfml-system
```
