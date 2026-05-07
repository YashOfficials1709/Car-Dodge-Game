#include "Car.h"

Car::Car(float x, float y, float w, float h, sf::Color color, float spd)
    : speed(spd), active(true) {
    shape.setSize({w, h});
    shape.setPosition({x, y});
    shape.setFillColor(color);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(1.f);
}

void Car::draw(sf::RenderWindow& window) {
    if (active) window.draw(shape);
}

sf::FloatRect Car::getBounds() const { return shape.getGlobalBounds(); }
bool Car::isActive() const { return active; }
void Car::setActive(bool a) { active = a; }
float Car::getY() const { return shape.getPosition().y; }
void Car::setSpeed(float s) { speed = s; }
float Car::getSpeed() const { return speed; }