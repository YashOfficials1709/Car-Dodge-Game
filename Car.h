#pragma once
#include <SFML/Graphics.hpp>

class Car {
protected:
    sf::RectangleShape shape;
    float speed;
    bool active;
public:
    Car(float x, float y, float w, float h, sf::Color color, float spd);
    virtual ~Car() = default;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isActive() const;
    void setActive(bool a);
    float getY() const;
    void setSpeed(float s);
    float getSpeed() const;
};