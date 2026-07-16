//
// Created by jcksn on 7/13/2026.
//

#ifndef TESTGAME_INIT_PLAYER_H
#define TESTGAME_INIT_PLAYER_H

#include "../Entities/Entities.h"

Player InitializePlayer(const sf::Texture& texture, const sf::Vector2f pos);
Enemy InitializeEnemies(const sf::Texture& texture);


#endif //TESTGAME_INIT_PLAYER_H