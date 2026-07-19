//
// Created by jcksn on 7/13/2026.
//

#include "Init.h"

#include "SFML/Graphics/Texture.hpp"

Player Spawn::InitializePlayer(const sf::Texture& texture, const sf::Vector2f& pos) {
    const sf::Vector2u textureSize = texture.getSize();

    sf::Sprite sprite{texture};

    sprite.setOrigin({static_cast<float>(textureSize.x) / 2.f, static_cast<float>(textureSize.y) / 2.f});
    sprite.setScale({0.25f, 0.25f});

    return Player{sprite, pos};
}

Enemy Spawn::InitializeEnemy(const sf::Texture& texture, const sf::Vector2f& pos) {
    const sf::Vector2u  textureSize = texture.getSize();

    sf::Sprite sprite{texture};

    sprite.setOrigin({static_cast<float>(textureSize.x) / 2.f, static_cast<float>(textureSize.y) / 2.f});
    sprite.setScale({0.25f, 0.25f});

    return Enemy{sprite, pos};
}
// Enemy InitializeEnemies(const sf::Texture& texture) {
//     return Enemy{sprite};
// }