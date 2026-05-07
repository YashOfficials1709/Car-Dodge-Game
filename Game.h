#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include "PlayerCar.h"
#include "EnemyCar.h"

enum class GameState { MENU, PLAYING, PAUSED, GAMEOVER };
enum class Difficulty { EASY, MEDIUM, HARD };

class Game {
    sf::RenderWindow window;
    sf::Font font;
    sf::Clock clock, spawnClock, speedClock;

    // Road
    float roadLeft, roadRight, roadWidth;
    int numLanes;
    float laneWidth;

    // Game objects
    std::unique_ptr<PlayerCar> player;
    std::vector<std::unique_ptr<EnemyCar>> enemies;

    // State
    GameState state;
    Difficulty difficulty;
    int score;
    float baseSpeed, currentSpeed;
    float spawnInterval;
    int speedLevel;
    bool highScoreBeaten;
    int highScore;

    // UI
    sf::Text scoreText{font};
    sf::Text speedText{font};
    sf::Text stateText{font};
    sf::Text diffText{font};
    sf::RectangleShape roadShape;
    std::vector<sf::RectangleShape> laneMarkers;

    // Audio
    sf::Music bgMusic;

    void initWindow();
    void initRoad();
    void initHUD();
    void initAudio();
    void handleEvents();
    void update(float dt);
    void render();
    void spawnEnemy();
    void checkCollisions();
    void updateHUD();
    void drawRoad();
    void drawLaneMarkers(float dt);
    void drawMenu();
    void drawPause();
    void drawGameOver();
    void resetGame();
    void updateSpeed(float elapsed);
    float getLaneX(int lane);
    void selectDifficulty(Difficulty d);

    float markerOffset = 0.f;
public:
    Game();
    void run();
};