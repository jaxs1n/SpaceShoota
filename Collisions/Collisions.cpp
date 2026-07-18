//
// Created by jcksn on 7/18/2026.
//

#include "Collisions.h"

bool CircleToCircle(const sf::CircleShape& circle1, const sf::CircleShape& circle2) {
    const float dx = circle1.getPosition().x - circle2.getPosition().x;
    const float dy = circle1.getPosition().y - circle2.getPosition().y;

    const float radius_squared = (circle1.getRadius() + circle2.getRadius()) * (circle1.getRadius() + circle2.getRadius());

    if (const float distance_squared = (dx * dx) + (dy * dy); distance_squared < radius_squared) {
        return true;
    }
    return false;
}
