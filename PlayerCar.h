#pragma once
#include "Car.h"
#include <optional>

class PlayerCar : public Car {
    float roadLeft, roadRight;
    sf::Texture texture;
    std::optional<sf::Sprite> sprite;
public:
    PlayerCar(float x, float y, float roadL, float roadR);
    void update(float dt) override;
    void handleInput();
    void draw(sf::RenderWindow& window) override;
    void reset(float x, float y);
};