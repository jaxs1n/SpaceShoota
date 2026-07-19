//
// Created by jcksn on 7/13/2026.
//

#include "steering.h"
#include <cmath>
#include <iostream>
#include <ostream>

#include "SFML/Window/Mouse.hpp"

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

    if (const float lengthSquared = difference.x * difference.x + difference.y * difference.y; lengthSquared < 0.0001f) {
        return -1.f;
    }

    const float angleRadians = std::atan2(difference.y, difference.x);
    constexpr float radiansToDegrees = 180.f / 3.14159265f;

    return angleRadians * radiansToDegrees + 90.f;
}

sf::Vector2f Steering::ClosestEnemy(const sf::Vector2f player_pos, const std::vector<Enemy>& enemies) {
    float closestDistanceSquared = std::numeric_limits<float>::max();
    sf::Vector2f closest_enemy_pos = {-1, -1};
    for (auto& enemy : enemies) {
        const sf::Vector2f difference = enemy.GetPosition() - player_pos;

        const float distanceSquared =
            difference.x * difference.x +
            difference.y * difference.y;

        if (distanceSquared < closestDistanceSquared) {
            closestDistanceSquared = distanceSquared;
            closest_enemy_pos = enemy.GetPosition();
        }
    }
    return closest_enemy_pos;
}

float Steering::PlayerPointToMouse(const sf::Vector2f& mouse_position, const sf::Vector2f& player_pos) {
    const sf::Vector2f difference = player_pos - mouse_position;

    const float angleRadians = std::atan2(difference.y, difference.x);
    constexpr float radiansToDegrees = 180.f / 3.14159265f;

    return angleRadians * radiansToDegrees - 90.f;
}

sf::Vector2f Steering::PlayerDirectionToMouse(const sf::Vector2f& mouse_position, const sf::Vector2f& player_pos) {
    sf::Vector2f direction = mouse_position - player_pos;

    const float length = std::sqrt(
        direction.x * direction.x +
        direction.y * direction.y
    );

    if (length != 0.f) {
        direction /= length;
    }

    return direction;
}
