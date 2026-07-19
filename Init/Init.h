//
// Created by jcksn on 7/13/2026.
//

#ifndef TESTGAME_INIT_PLAYER_H
#define TESTGAME_INIT_PLAYER_H

#include "../Entities/Entities.h"

namespace Spawn {
    Player InitializePlayer(const sf::Texture& texture, const sf::Vector2f& pos);
    Enemy InitializeEnemy(const sf::Texture& texture, const sf::Vector2f& pos);
}

#endif //TESTGAME_INIT_PLAYER_H