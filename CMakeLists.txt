cmake_minimum_required(VERSION 3.10)
project(CarDodgeGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

add_executable(CarDodgeGame
    main.cpp
    Car.cpp
    PlayerCar.cpp
    EnemyCar.cpp
    Game.cpp
)

target_link_libraries(CarDodgeGame
    sfml-graphics sfml-window sfml-system sfml-audio
)