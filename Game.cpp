#include "Game.h"
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Game::Game() : state(GameState::MENU), difficulty(Difficulty::MEDIUM),
    score(0), baseSpeed(200.f), currentSpeed(200.f),
    spawnInterval(1.5f), speedLevel(1), highScore(0), highScoreBeaten(false)
     {
    srand((unsigned)time(nullptr));
    initWindow();
    initRoad();
    initHUD();
    initAudio();
}

void Game::initWindow() {
    window.create(sf::VideoMode({400u, 600u}), "Car Dodge Game", sf::Style::Close);
    window.setFramerateLimit(60);
}

void Game::initRoad() {
    roadWidth = 240.f;
    roadLeft = (400.f - roadWidth) / 2.f;
    roadRight = roadLeft + roadWidth;
    numLanes = 4;
    laneWidth = roadWidth / numLanes;

    roadShape.setSize({roadWidth, 600.f});
    roadShape.setPosition({roadLeft, 0.f});
    roadShape.setFillColor(sf::Color(40, 40, 40));

    // Lane markers (dashed lines between lanes)
    for (int i = 1; i < numLanes; i++) {
        for (int j = 0; j < 10; j++) {
            sf::RectangleShape marker({4.f, 30.f});
            marker.setPosition({roadLeft + i * laneWidth - 2.f, j * 70.f});
            marker.setFillColor(sf::Color::White);
            laneMarkers.push_back(marker);
        }
    }

    float px = roadLeft + roadWidth / 2.f - 20.f;
    float py = 600.f - 100.f;
    player = std::make_unique<PlayerCar>(px, py, roadLeft, roadRight);
}

void Game::initHUD() {
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        // try fallback
        font.openFromFile("C:/Windows/Fonts/tahoma.ttf");
    }
    scoreText.setFont(font); scoreText.setCharacterSize(18);
    scoreText.setFillColor(sf::Color::White); scoreText.setPosition({roadLeft + 5, 5.f});

    speedText.setFont(font); speedText.setCharacterSize(18);
    speedText.setFillColor(sf::Color::White); speedText.setPosition({roadRight - 100, 5.f});

    stateText.setFont(font); stateText.setCharacterSize(28);
    stateText.setFillColor(sf::Color::Yellow);

    diffText.setFont(font); diffText.setCharacterSize(20);
    diffText.setFillColor(sf::Color::Cyan);
}

void Game::initAudio() {
    // bgMusic and sounds are optional; skip if files missing
    bgMusic.openFromFile("assets/bg.ogg"); // won't crash if missing
    bgMusic.setLooping(true);
    // bgMusic.play();
}

float Game::getLaneX(int lane) {
    return roadLeft + lane * laneWidth + laneWidth / 2.f - 20.f;
}

void Game::selectDifficulty(Difficulty d) {
    difficulty = d;
    switch (d) {
        case Difficulty::EASY:   baseSpeed = 150.f; spawnInterval = 2.0f; break;
        case Difficulty::MEDIUM: baseSpeed = 200.f; spawnInterval = 1.5f; break;
        case Difficulty::HARD:   baseSpeed = 280.f; spawnInterval = 0.9f; break;
    }
    currentSpeed = baseSpeed;
}

void Game::resetGame() {
    enemies.clear();
    score = 0; speedLevel = 1;
    currentSpeed = baseSpeed;
    float px = roadLeft + roadWidth / 2.f - 20.f;
    player->reset(px, 600.f - 100.f);
    clock.restart(); spawnClock.restart(); speedClock.restart();
    state = GameState::PLAYING;
}

void Game::spawnEnemy() {
    int lane = rand() % numLanes;
    float spd = currentSpeed + (rand() % 60 - 30);
    if (difficulty == Difficulty::HARD) spd += 50.f;
    enemies.push_back(std::make_unique<EnemyCar>(getLaneX(lane), -80.f, spd, 600.f));
}

void Game::updateSpeed(float elapsed) {
    // Increase speed every 10 seconds
    int newLevel = 1 + (int)(elapsed / 10.f);
    if (newLevel != speedLevel) {
        speedLevel = newLevel;
        currentSpeed = baseSpeed + (speedLevel - 1) * 30.f;
        // Reduce spawn interval slightly
        spawnInterval = std::max(0.5f, spawnInterval - 0.05f);
    }
}

void Game::checkCollisions() {
    auto pb = player->getBounds();
    for (auto& e : enemies) {
        if (e->isActive() && pb.findIntersection(e->getBounds()).has_value()) {
            state = GameState::GAMEOVER;
            if (score > highScore) { highScore = score; highScoreBeaten = true; }
            return;
        }
    }
}

void Game::updateHUD() {
    std::ostringstream ss;
    ss << "SCORE: " << score;
    scoreText.setString(ss.str());
    ss.str(""); ss << "SPEED: " << speedLevel;
    speedText.setString(ss.str());
}

void Game::drawRoad() {
    window.draw(roadShape);
}

void Game::drawLaneMarkers(float dt) {
    markerOffset += currentSpeed * dt;
    if (markerOffset > 70.f) markerOffset = 0.f;
    for (int i = 1; i < numLanes; i++) {
        for (int j = -1; j < 10; j++) {
            sf::RectangleShape marker({4.f, 30.f});
            marker.setPosition({roadLeft + i * laneWidth - 2.f, j * 70.f + markerOffset});
            marker.setFillColor(sf::Color(180, 180, 180));
            window.draw(marker);
        }
    }
}

void Game::drawMenu() {
    sf::Text title(font, "CAR DODGE", 36);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({110.f, 100.f});

    sf::Text opt1(font, "1 - Easy", 22);
    opt1.setFillColor(sf::Color::Green);
    opt1.setPosition({150.f, 220.f});

    sf::Text opt2(font, "2 - Medium", 22);
    opt2.setFillColor(sf::Color::Yellow);
    opt2.setPosition({150.f, 270.f});

    sf::Text opt3(font, "3 - Hard", 22);
    opt3.setFillColor(sf::Color::Red);
    opt3.setPosition({150.f, 320.f});

    sf::Text opt4(font, "Best: " + std::to_string(highScore), 20);
    opt4.setFillColor(sf::Color::Cyan);
    opt4.setPosition({150.f, 400.f});

    window.draw(title); window.draw(opt1);
    window.draw(opt2); window.draw(opt3); window.draw(opt4);
}

void Game::drawPause() {
    sf::RectangleShape overlay({400.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 120));
    window.draw(overlay);
    stateText.setString("PAUSED\nP to Resume");
    stateText.setPosition({120.f, 250.f});
    window.draw(stateText);
}

void Game::drawGameOver() {
    sf::RectangleShape overlay({400.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    stateText.setCharacterSize(32);
    stateText.setString("GAME OVER");
    stateText.setPosition({110.f, 200.f});
    window.draw(stateText);

    sf::Text info(font, "", 20);
    info.setFillColor(sf::Color::White);
    std::ostringstream ss;
    ss << "Score: " << score << "\nBest:  " << highScore;
    if (highScoreBeaten) ss << " (NEW!)";
    ss << "\n\nR - Restart\nM - Menu\nESC - Quit";
    info.setString(ss.str());
    info.setPosition({130.f, 260.f});
    window.draw(info);
}

void Game::handleEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            continue;
        }

        if (event->is<sf::Event::KeyPressed>()) {
            auto keyEvent = event->getIf<sf::Event::KeyPressed>();
            if (!keyEvent) continue;
            auto key = keyEvent->code;

            if (state == GameState::MENU) {
                if (key == sf::Keyboard::Key::Num1) { selectDifficulty(Difficulty::EASY); resetGame(); }
                if (key == sf::Keyboard::Key::Num2) { selectDifficulty(Difficulty::MEDIUM); resetGame(); }
                if (key == sf::Keyboard::Key::Num3) { selectDifficulty(Difficulty::HARD); resetGame(); }
            }
            if (state == GameState::PLAYING && key == sf::Keyboard::Key::P)
                state = GameState::PAUSED;
            if (state == GameState::PAUSED && key == sf::Keyboard::Key::P)
                state = GameState::PLAYING;
            if (state == GameState::GAMEOVER) {
                highScoreBeaten = false;
                if (key == sf::Keyboard::Key::R) resetGame();
                if (key == sf::Keyboard::Key::M) state = GameState::MENU;
            }
            if (key == sf::Keyboard::Key::Escape) window.close();
        }
    }
}

void Game::update(float dt) {
    if (state != GameState::PLAYING) return;
    float elapsed = clock.getElapsedTime().asSeconds();
    updateSpeed(elapsed);
    score = (int)(elapsed * 10.f);

    player->update(dt);

    if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
        spawnEnemy();
        spawnClock.restart();
    }

    for (auto& e : enemies) e->update(dt);
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<EnemyCar>& e){ return e->isOffScreen(); }), enemies.end());

    checkCollisions();
    updateHUD();
}

void Game::render() {
    window.clear(sf::Color(20, 20, 20));
    float dt = 1.f / 60.f;

    if (state == GameState::MENU) {
        drawMenu();
    } else {
        drawRoad();
        drawLaneMarkers(dt);
        for (auto& e : enemies) e->draw(window);
        player->draw(window);
        window.draw(scoreText);
        window.draw(speedText);
        if (state == GameState::PAUSED) drawPause();
        if (state == GameState::GAMEOVER) drawGameOver();
    }
    window.display();
}

void Game::run() {
    sf::Clock frameClock;
    while (window.isOpen()) {
        float dt = frameClock.restart().asSeconds();
        handleEvents();
        update(dt);
        render();
    }
}