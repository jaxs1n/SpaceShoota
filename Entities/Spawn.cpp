//
// Created by jcksn on 7/19/2026.
//

#include "Spawn.h"

#include <random>


sf::Vector2f EnemySpawnLocation(const sf::Vector2f& player_pos) {
    constexpr float innerDistance = 750.f;
    constexpr float outerDistance = 1000.f;

    static std::mt19937 randomEngine{std::random_device{}()};
    std::uniform_int_distribution<int> sideDistribution{0, 3};
    std::uniform_real_distribution<float> offsetDistribution{
        -outerDistance,
        outerDistance
    };
    std::uniform_real_distribution<float> distanceDistribution{
        innerDistance,
        outerDistance
    };

    const int side = sideDistribution(randomEngine);
    const float offset = offsetDistribution(randomEngine);
    const float distance = distanceDistribution(randomEngine);

    switch (side) {
        case 0:
            return {
                player_pos.x + offset,
                player_pos.y - distance
            };

        case 1:
            return {
                player_pos.x + offset,
                player_pos.y + distance
            };

        case 2:
            return {
                player_pos.x + distance,
                player_pos.y + offset
            };

        case 3:
            return {
                player_pos.x - distance,
                player_pos.y + offset
            };

        default:
            return player_pos;
    }
}
