//
// Created by jcksn on 7/13/2026.
//

#include "steering.h"
#include <cmath>
#include <optional>

#include "Entities.h"

sf::Vector2f Steering::EnemyToPlayer(const sf::Vector2f player_pos, const sf::Vector2f enemy_pos) {
    const sf::Vector2f difference = enemy_pos - player_pos;

    const float length = std::sqrt(
        difference.x * difference.x +
        difference.y * difference.y
    );

    if (length == 0.0f) {
        return {0.0f, 0.0f};
    }

    return difference / length;
}

float Steering::PlayerPointToEnemy(const sf::Vector2f player_pos, const sf::Vector2f enemy_pos) {
    const sf::Vector2f difference = enemy_pos - player_pos;

    const float lengthSquared =
        difference.x * difference.x +
        difference.y * difference.y;

    if (lengthSquared < 0.0001f) {
        return -1.f;
    }

    const float angleRadians =
        std::atan2(difference.y, difference.x);

    constexpr float radiansToDegrees =
        180.f / 3.14159265f;

    return angleRadians * radiansToDegrees + 90.f;
}

sf::Vector2f Steering::ClosestEnemy(const sf::Vector2f player_pos) {


    return {-1, -1};
}
