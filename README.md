# Battleship Game in C

<img align="right" src="https://i.imgur.com/5ENqoMn.png" height="150px">

Welcome to the Battleship Game in C! In this version of the classic battleship game, players take turns guessing the
location of each other's ships on a grid. When a player misses a ship, a mine is placed on the board. The game ends when
all of one player's ships have been sunk. Players can move their ships and attack their opponent's ships on each turn.
The game also includes three types of AI that players can compete against.

![GitHub release (latest by date)](https://img.shields.io/github/v/release/TriForMine/battleship)
![GitHub](https://img.shields.io/github/license/TriForMine/battleship)
![GitHub last commit](https://img.shields.io/github/last-commit/TriForMine/battleship) <br/>
[![test](https://github.com/TriForMine/battleship/actions/workflows/test.yml/badge.svg)](https://github.com/TriForMine/battleship/actions/workflows/test.yml)
[![pre-release](https://github.com/TriForMine/battleship/actions/workflows/pre-release.yml/badge.svg)](https://github.com/TriForMine/battleship/actions/workflows/pre-release.yml)
[![DeepSource](https://deepsource.io/gh/TriForMine/battleship.svg/?label=active+issues&show_trend=true&token=46Xb9KY7rG195b7ilvE_jl6i)](https://deepsource.io/gh/TriForMine/battleship/?ref=repository-badge)

## Gameplay

In Battleship, players take turns guessing the location of each other's ships on a grid. The size of the grid can be
customized at the start of the game.

Each player places their ships on the board and then takes turns guessing the location of the opponent's ships. If a
player hits a ship, the turn ends and the opponent gets to go. If a player misses a ship, a mine is placed on the board.
If a player hits a mine, the turn ends and the opponent gets to go.

The game ends when all of one player's ships have been sunk. On each turn, a player can either move one of their ships
or attack an opponent's ship. If two ships collide on the same space, both ships will be damaged and their movements
will be cancelled. If a ship moves onto a mine, it will be damaged. The game ends when one player has no ships left.

## Rules

- Players take turns playing the game.
- If a player misses a shot, they will place a mine on the board.
- If a ship moves onto a mine, it will be damaged.
- If two ships collide on the same space, both ships will be damaged and their movements will be cancelled.
- On each turn, a player can either move one of their ships or attack an opponent's ship.
- The game ends when one player has no ships left.

## AI

The game includes 3 types of AI:

### Random AI

This AI fires at random coordinates each turn.

### Hunt and Target

This AI fires in a grid form, and when there is a hit, fires on the adjacent cell until the ship is sunk.

### Probability Targeting

This AI calculates at every turn where the highest probability for a ship to be is, and fires at that point. When in
hunting mode, it will calculate all the possibilities for the hit ship to be placed and eliminate it quickly. This AI is
the most difficult to beat.

## Installation

To install and run the game, you have two options:

1. Clone the repository to your local machine and compile the source code using the Makefile.
2. Download a release from the [releases page](https://github.com/TriForMine/battleship/releases) and run the game.

### Option 1: Install using the Makefile

1. Clone the repository to your local machine:

```sh
git clone https://github.com/TriForMine/battleship.git
```

2. Navigate to the directory where you cloned the repository:

```sh
cd battleship
```

3. Compile the source code using the Makefile:

```sh
make release
```

To build the game with the debug mode, use `make debug` instead. This will show a map of the probabilities for the
probability targeting mode.

4. Run the game:

```sh
./build/battleship
```

### Option 2: Install using CMake and Ninja

1. Clone the repository to your local machine:

```sh
git clone https://github.com/TriForMine/battleship.git
```

2. Navigate to the directory where you cloned the repository:

```sh
cd battleship
```

3. Compile the source code using CMake and Ninja:

```sh
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build build
```

4. Run the game:

```sh
./build/battleship
```

## Contributions

We welcome contributions to the project! If you would like to contribute, please follow these steps:

1. Read the [CONTRIBUTING.md](CONTRIBUTING.md) file for guidelines on contributing to the project.
2. Fork the repository.
3. Create a new branch for your changes.
4. Make your changes.
5. Commit your changes and push them to your fork.
6. Create a pull request to merge your changes into the main branch.

We will review your pull request and provide feedback as needed. Thank you for considering contributing to the project!

## License

This project is licensed under the Apache-2.0 license. See the [LICENSE](LICENSE) file for more information.
