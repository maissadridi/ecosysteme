# Simulation d'Écosystème
Une simulation en C++/SFML d’un écosystème prédateur-proie avec des loups, des moutons, de l’herbe et des minéraux.

Fonctionnalités

🐑 Moutons qui mangent de l’herbe

🐺 Loups qui chassent les moutons

🌱 Herbe qui repousse grâce aux minéraux

⚖️ Dynamique d’écosystème équilibrée

🎨 Interface visuelle avec SFML

📊 Suivi des statistiques en temps réel

Prérequis
C++14 ou version plus récente

Bibliothèque SFML

Git (pour le contrôle de version)
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
