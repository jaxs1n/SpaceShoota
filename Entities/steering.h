//
// Created by jcksn on 7/13/2026.
//

#ifndef TESTGAME_STEERING_H
#define TESTGAME_STEERING_H

#include "SFML/System/Vector2.hpp"

namespace Steering {
    sf::Vector2f EnemyToPlayer(sf::Vector2f player_pos, sf::Vector2f enemy_pos);
    sf::Vector2f PlayerMovement();
    float PlayerPointToEnemy(sf::Vector2f player_pos, sf::Vector2f enemy_pos);
    sf::Vector2f ClosestEnemy(sf::Vector2f player_pos);
}
#endif //TESTGAME_STEERING_H