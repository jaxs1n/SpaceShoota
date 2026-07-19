#ifndef TESTGAME_ENTITIES_H
#define TESTGAME_ENTITIES_H

#include <list>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"

class Enemy;

class CharacterEntity {
public:
    CharacterEntity(sf::CircleShape shape, int health, sf::Vector2f position);

    void Update(float dt);
    void Die();
    void TakeDamage(int damage);

    [[nodiscard]] bool IsAlive() const;
    [[nodiscard]] int GetHealth() const;
    [[nodiscard]] int GetId() const;

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
    int Id{};
    bool alive{true};

    sf::Vector2f position{};
    sf::Vector2f velocity{};
    sf::Vector2f acceleration{};

    sf::CircleShape shape;
};

class BulletEntity {
public:

    BulletEntity(sf::CircleShape shape, const sf::Vector2f &starting_pos, const sf::Vector2f &starting_velo, int damage);
    BulletEntity(sf::CircleShape  shape, const sf::Vector2f &starting_pos, int damage);
    void Update(float dt);

    const sf::CircleShape& GetShape() const;
    sf::Vector2f GetPosition() const;
    int GetId() const;

private:
    int damage{};
    sf::CircleShape hitbox;
    sf::Vector2f position{};
    sf::Vector2f velocity{};
    static constexpr float bullet_speed = 800.f;
    const int Id{};
};

class Player {
public:
    explicit Player(sf::Sprite sprite, const sf::Vector2f& starting_pos);

    void Update(float dt, sf::Vector2f enemy_pos, const sf::RenderWindow& window);
    void SetVelocity(sf::Vector2f velocity);
    void SetAcceleration(sf::Vector2f acceleration);

    void Shoot(const sf::Vector2f& closest_enemy_pos);
    void Shoot();
    void RemoveBullet(int bulletId);

    [[nodiscard]] sf::Vector2f GetPosition() const;
    [[nodiscard]] const sf::CircleShape& GetShape() const;
    [[nodiscard]] const sf::Sprite& GetSprite() const;
    std::list<BulletEntity>& GetBullets();


private:
    CharacterEntity entity;
    std::list<BulletEntity> bullets{};
    int damage{1};
    sf::Sprite sprite;
    bool LookingAtEnemy{false};
    float shoot_cooldown{};
};

class Enemy {
public:
    explicit Enemy(sf::Sprite sprite, const sf::Vector2f& starting_pos);

    void Update(float dt, const sf::Vector2f& player_pos);
    void SetVelocity(sf::Vector2f velocity);
    void SetAcceleration(sf::Vector2f acceleration);

    [[nodiscard]] sf::Vector2f GetPosition() const;
    [[nodiscard]] const sf::CircleShape& GetShape() const;
    [[nodiscard]] const sf::Sprite& GetSprite() const;

    int GetEnemyId() const;

private:
    CharacterEntity entity;
    sf::Sprite sprite;
};

#endif