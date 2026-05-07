#pragma once
#include "Car.h"
#include <SFML/Graphics.hpp>

class EnemyCar : public Car {
    float screenHeight;
    static const sf::Color COLORS[4];
    sf::Texture texture;
    std::optional<sf::Sprite> sprite;
public:
    EnemyCar(float x, float y, float spd, float scrH);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    bool isOffScreen() const;
};