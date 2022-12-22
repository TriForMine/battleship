# Battleship Game in C

<img align="right" src="https://i.imgur.com/5ENqoMn.png" height="150px">

Welcome to the Battleship Game in C! This is a variation of the classic battleship game where you can move your ships and when you miss a ship, a mine is placed on the board.

![GitHub release (latest by date)](https://img.shields.io/github/v/release/TriForMine/battleship)
![GitHub](https://img.shields.io/github/license/TriForMine/battleship)
![GitHub last commit](https://img.shields.io/github/last-commit/TriForMine/battleship) <br/>
[![test](https://github.com/TriForMine/battleship/actions/workflows/test.yml/badge.svg)](https://github.com/TriForMine/battleship/actions/workflows/test.yml)
[![pre-release](https://github.com/TriForMine/battleship/actions/workflows/pre-release.yml/badge.svg)](https://github.com/TriForMine/battleship/actions/workflows/pre-release.yml)
[![DeepSource](https://deepsource.io/gh/TriForMine/battleship.svg/?label=active+issues&show_trend=true&token=46Xb9KY7rG195b7ilvE_jl6i)](https://deepsource.io/gh/TriForMine/battleship/?ref=repository-badge)

## Installation

To install and run the game, follow these steps:

1. Clone the repository to your local machine
```sh
git clone https://github.com/TriForMine/battleship.git
```
2. Navigate to the directory where you cloned the repository
```sh
cd battleship
```
3. Compile the source code using the Makefile
```sh
make
```
4. Run the game
```
./build/battleship
```

## Gameplay

The game is played on a 10x10 grid. Each player places their ships on the board and then takes turns guessing the location of the opponent's ships. If a player hits a ship, the turn ends and the opponent gets to go. If a player misses a ship, a mine is placed on the board. If a player hits a mine, the turn ends and the opponent gets to go. The game ends when all of one player's ships have been sunk.

## Contributions

If you would like to contribute to the project, please fork the repository and create a pull request with your changes.

## License

This project is licensed under the Apache-2.0 license. See the [LICENSE](LICENSE) file for more information.
