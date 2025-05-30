# Simulation d'Écosystème
Une simulation en C++/SFML d’un écosystème prédateur-proie avec des loups, des moutons, de l’herbe et des minéraux.

## 1. Introduction
Ce rapport présente une analyse détaillée du code source d'une simulation d'écosystème implémentant le principe de proie-prédateur (moutons-loups) avec des mécanismes de reproduction, de nutrition et de mortalité.

### 2. Fonctionnalités

🐑 Moutons qui mangent de l’herbe

🐺 Loups qui chassent les moutons

🌱 Herbe qui repousse grâce aux minéraux

⚖️ Dynamique d’écosystème équilibrée

🎨 Interface visuelle avec SFML

📊 Suivi des statistiques en temps réel

## 3. Mécanismes Clés Implémentés

### 3.1. Cycle de Vie des Entités

- **Vieillissement** : Chaque entité a une durée de vie maximale
- **Énergie** : Gérée via un système de points (alimentation, reproduction)
- **Faim** : Compteur de tours sans manger menant à la mort
- **Reproduction** : Mécanisme sexué avec gestation et naissance

### 3.2. Comportements Spécifiques

- **Moutons** :
    - Se nourrissent d'herbe
    - Reproduction plus rapide que les loups
    - Déplacement semi-aléatoire avec tendance vers la nourriture
- **Loups** :
    - Se nourrissent de moutons
    - Reproduction plus lente mais plus durable
    - Déplacement avec recherche proactive de proies

### 3.3. Gestion de l'Environnement

- **Herbe** :
    - Croissance progressive après consommation
    - Différents stades de maturité visibles graphiquement

## 4. Points Forts du Code

1. **Conception Orientée Objet** :
    - Hiérarchie de classes bien pensée
    - Utilisation de polymorphisme pour les comportements spécifiques
    - Encapsulation des propriétés des entités
2. **Gestion du Temps** :
    - Système de tours avec possibilité de pause
    - Vitesse de simulation réglable
3. **Interface Graphique** :
    - Visualisation claire avec SFML
    - Codage couleur pour les différents états
    - Affichage des statistiques en temps réel
4. **Équilibre Écologique** :
    - Paramètres ajustables pour les constantes de simulation
    - Mécanismes de régulation populationnelle

## 5. Améliorations Possibles

1. **Optimisation des Performances** :
    - Utilisation de structures spatiales (quadtree) pour les collisions
    - Gestion plus efficace des recherches d'entités voisines
2. **Fonctionnalités Supplémentaires** :
    - Sauvegarde/chargement de l'état de la simulation
    - Graphiques d'évolution des populations
    - Paramétrage dynamique des constantes
3. **Améliorations Graphiques** :
    - Sprites au lieu de formes géométriques simples
    - Animations pour les déplacements et actions
4. **Robustesse** :
    - Gestion des erreurs de fichier pour la police
    - Vérification des limites des tableaux


### Prérequis
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

##  Conclusion

Ce code constitue une excellente base pour une simulation écologique réaliste. Il démontre une bonne compréhension des principes de modélisation des écosystèmes et de programmation orientée objet. Les mécanismes fondamentaux sont correctement implémentés et l'interface permet une observation aisée de la dynamique du système.

Le programme pourrait être enrichi avec les améliorations proposées, mais dans son état actuel, il remplit déjà efficacement son rôle de simulateur d'écosystème proie-prédateur.

