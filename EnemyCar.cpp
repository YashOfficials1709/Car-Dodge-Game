#include "EnemyCar.h"

const sf::Color EnemyCar::COLORS[4] = {
    sf::Color::Red,
    sf::Color::Yellow,
    sf::Color(255, 165, 0),   // Orange
    sf::Color::Cyan
};

static const char* ENEMY_TEXTURES[4] = {
    "assets/RedCar1.png",
    "assets/RedCar2.png",
    "assets/YellowCar1.png",
    "assets/YellowCar2.png"
};

EnemyCar::EnemyCar(float x, float y, float spd, float scrH)
    : Car(x, y, 40.f, 70.f, sf::Color::Red, spd), screenHeight(scrH) {
    int idx = rand() % 4;
    if (texture.loadFromFile(ENEMY_TEXTURES[idx])) {
        sprite.emplace(texture);
        sprite->setScale({40.f / texture.getSize().x, 70.f / texture.getSize().y});
        sprite->setPosition({x, y});
    } else {
        shape.setFillColor(COLORS[idx]);
    }
}

void EnemyCar::update(float dt) {
    if (active) {
        shape.move(sf::Vector2f(0.f, speed * dt));
        if (sprite) sprite->setPosition(shape.getPosition());
    }
}

void EnemyCar::draw(sf::RenderWindow& window) {
    if (active) {
        if (sprite) window.draw(*sprite);
        else window.draw(shape);
    }
}

bool EnemyCar::isOffScreen() const {
    return shape.getPosition().y > screenHeight;
}