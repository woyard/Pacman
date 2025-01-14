## A simple game of Pacman 
implemented with C++ and Qt 
### Basics:
- arrow keys to move, 
- eat all dots to advance,
- avoid the ghosts
- eat the power pellets to eat the ghosts

### Features:
- my simple A* pathfinding engine
- 4 ghost types running A* pathfinding with different strategies
- .txt level files for easy level creation (I recommend https://asciiflow.com)
- high score keeping integrated in level file
- simple collision detection for all entities
- easily extensible class system for adding new entities

### level1_32x16.txt preview:
<img width="1021" alt="Snipaste_2025-01-07_17-37-49" src="https://github.com/user-attachments/assets/e9692b01-8c80-443c-8dcd-5171916ac4ae" />

### Compilation & Running (linux):
```bash
# get Qt the 'canonical way':
sudo apt install qt5-default
# but for me something like that worked: sudo apt install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
git clone https://github.com/woyard/Pacman
cd YOUR_PATH/Pacman
cmake .
make
./Pacman 
```
### Supports Launch Arguments:
```
arg1: int tileSize = 8
arg2: int frameRate = 60
arg3: string levelFile = "level1_32x16.txt"
arg4: bool debugView = false

```
```bash

./Pacman 8 60 "level1_32x16.txt"
```
### Level File - Minimum Requirements:
first row: 
```c++
- int width // number of characters in a row
- int height // number of rows 
- int highscore = 0 // highscore for the level (updated by the game)
```
rest of file:
```
width x height : grid of chars representing the level
```

the definitions for the chars are in [./LevelUtilities/tile_definitions.h](./LevelUtilities/tile_definitions.h)

decently robust -
<b> minimally requires only a Player and a Pellet to function </b>

### Level File Format:
```text
16 16 0
###.########.###
###.########.###
##...o.##.o...##
##.# #....# #.##
##.# ###### #.##
##.# D R  D #.##
##.# #ICPB# #.##
##.# ###### #.##
................
##.###.##.###.##
##.#...@....#.##
##.#.######.#.##
##.#.######.#.##
##...o....o...##
###.########.###
###.########.###
```

