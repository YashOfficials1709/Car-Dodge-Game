#include "PlayerCar.h"

PlayerCar::PlayerCar(float x, float y, float roadL, float roadR)
    : Car(x, y, 40.f, 70.f, sf::Color::White, 200.f),
      roadLeft(roadL), roadRight(roadR) {
    if (texture.loadFromFile("assets/WhiteCar.png")) {
        sprite.emplace(texture);
        sprite->setScale({40.f / texture.getSize().x, 70.f / texture.getSize().y});
        sprite->setPosition({x, y});
    }
}

void PlayerCar::update(float dt) {
    handleInput();
    auto pos = shape.getPosition();
    if (pos.x < roadLeft) shape.setPosition({roadLeft, pos.y});
    if (pos.x + 40.f > roadRight) shape.setPosition({roadRight - 40.f, pos.y});
    if (sprite) sprite->setPosition(shape.getPosition());
}

void PlayerCar::handleInput() {
    float dt = 1.f / 60.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        shape.move({-speed * dt, 0.f});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        shape.move({speed * dt, 0.f});
}

void PlayerCar::reset(float x, float y) {
    shape.setPosition({x, y});
    if (sprite) sprite->setPosition({x, y});
    active = true;
}

void PlayerCar::draw(sf::RenderWindow& window) {
    if (active) {
        if (sprite) window.draw(*sprite);
        else window.draw(shape);
    }
}
