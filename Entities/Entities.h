#ifndef TESTGAME_ENTITIES_H
#define TESTGAME_ENTITIES_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "SFML/Graphics/Sprite.hpp"

class CharacterEntity {
public:
    CharacterEntity(sf::CircleShape shape, int health, sf::Vector2f position);

    void Update(float dt);
    void Die();
    void TakeDamage(int damage);

    [[nodiscard]] bool IsAlive() const;
    [[nodiscard]] int GetHealth() const;

    [[nodiscard]] sf::Vector2f GetPosition() const;
    [[nodiscard]] sf::Vector2f GetVelocity() const;
    [[nodiscard]] sf::Vector2f GetAcceleration() const;

    [[nodiscard]] const sf::CircleShape& GetShape() const;
    [[nodiscard]] sf::CircleShape& GetShape();

    void SetPosition(sf::Vector2f position);
    void SetVelocity(sf::Vector2f velocity);
    void SetAcceleration(sf::Vector2f acceleration);

private:
    int health{};
    bool alive{true};

    sf::Vector2f position{};
    sf::Vector2f velocity{};
    sf::Vector2f acceleration{};

    sf::CircleShape shape;
};

class Player {
public:
    explicit Player(sf::Sprite  sprite, const sf::Vector2f& starting_pos);

    void Update(float dt, sf::Vector2f enemy_pos);
    void SetVelocity(sf::Vector2f velocity);
    void SetAcceleration(sf::Vector2f acceleration);

    [[nodiscard]] sf::Vector2f GetPosition() const;
    [[nodiscard]] const sf::CircleShape& GetShape() const;
    [[nodiscard]] const sf::Sprite& GetSprite() const;

private:
    CharacterEntity entity;
    sf::Sprite sprite;
};

class Enemy {
public:
    explicit Enemy(sf::Sprite  sprite);

    void Update(float dt, const sf::Vector2f& player_pos);
    void SetVelocity(sf::Vector2f velocity);
    void SetAcceleration(sf::Vector2f acceleration);

    [[nodiscard]] sf::Vector2f GetPosition() const;
    [[nodiscard]] const sf::CircleShape& GetShape() const;
    [[nodiscard]] const sf::Sprite& GetSprite() const;
private:
    CharacterEntity entity;
    sf::Sprite sprite;
};

#endif