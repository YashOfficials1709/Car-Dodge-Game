# Car Dodge Game

A simple car dodging game built with C++ and SFML 3.

## Gameplay

- Use the arrow keys or `A` / `D` to move the player car.
- Avoid enemy cars that spawn from the top of the screen.
- Select difficulty from the menu: Easy, Medium, Hard.
- The player car uses `assets/WhiteCar.png`.

## Project Structure

- `main.cpp` - entry point
- `Game.h` / `Game.cpp` - game loop, rendering, input, HUD, and state
- `Car.h` / `Car.cpp` - base car class
- `PlayerCar.h` / `PlayerCar.cpp` - player-specific movement and rendering
- `EnemyCar.h` / `EnemyCar.cpp` - enemy car behavior
- `assets/` - game art assets, including the white player car sprite
- `SFML-3.1.0/` - local SFML library sources and headers

## Build Instructions

### Requirements

- `g++` or a C++17 compatible compiler
- SFML 3 installed or available via the `SFML-3.1.0` folder

### Build with GCC

From the project root:

```powershell
cd /d c:\Project
 g++ main.cpp Game.cpp Car.cpp PlayerCar.cpp EnemyCar.cpp -o CarDodgeGame -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

### Run the game

```powershell
cd /d c:\Project
.\CarDodgeGame.exe
```

## Notes

- The player car sprite is loaded from `assets/WhiteCar.png`.
- Enemy cars attempt to load from the `assets/` folder, with colored shapes as a fallback.
- If SFML paths differ on your system, adjust include/library settings or use CMake.

## License

Use and modify this project freely for learning and experimentation.
