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
    void SetHealth(int health);
    void TakeDamage();
    void Update(float dt);
    void TakeDamage(int damage);

private:
    int health{};
    int Id{};

    sf::Vector2f position{};
    sf::Vector2f velocity{};
    sf::Vector2f acceleration{};

    sf::CircleShape shape;
};

class BulletEntity {
public:

    BulletEntity(sf::CircleShape shape, const sf::Vector2f &starting_pos, const sf::Vector2f &starting_velo, int damage, sf::Sprite sprite);
    void Update(float dt);

    const sf::CircleShape& GetShape() const;
    const sf::Sprite& GetSprite() const;
    sf::Vector2f GetPosition() const;
    int GetId() const;

private:
    int damage{};
    sf::CircleShape hitbox;
    sf::Vector2f position{};
    sf::Vector2f velocity{};
    sf::Sprite sprite;
    static constexpr float bullet_speed = 1400.f;
    const int Id{};
};

class Player {
public:
    explicit Player(sf::Sprite sprite, const sf::Vector2f& starting_pos);

    void Update(float dt, const sf::RenderWindow& window);
    void SetVelocity(sf::Vector2f velocity);
    void SetAcceleration(sf::Vector2f acceleration);
    void SetPosition(sf::Vector2f position);

    void TakeDamage();


    void SetDamage(int dmg);

    void Shoot(const sf::Vector2f& closest_enemy_pos);
    void RemoveBullet(int bulletId);

    [[nodiscard]] sf::Vector2f GetPosition() const;
    [[nodiscard]] const sf::CircleShape& GetShape() const;
    [[nodiscard]] const sf::Sprite& GetSprite() const;
    [[nodiscard]] int GetDamage() const;
    [[nodiscard]] CharacterEntity& GetEntity();

    std::list<BulletEntity>& GetBullets();
    [[nodiscard]] const std::list<BulletEntity>& GetBullets() const;

private:
    CharacterEntity entity;
    std::list<BulletEntity> bullets{};
    int damage{1};
    sf::Sprite sprite;
    float shoot_cooldown{0};
};

class Enemy {
public:
    explicit Enemy(sf::Sprite sprite, const sf::Vector2f& starting_pos);

    void Update(float dt, const sf::Vector2f& player_pos);
    void SetVelocity(sf::Vector2f velocity);
    void SetAcceleration(sf::Vector2f acceleration);
    void Shoot(const sf::Vector2f& player_position);
    void PointToPlayer(const sf::Vector2f& player_position);

    [[nodiscard]] sf::Vector2f GetPosition() const;
    [[nodiscard]] const sf::CircleShape& GetShape() const;
    [[nodiscard]] const sf::Sprite& GetSprite() const;
    [[nodiscard]] CharacterEntity& GetEntity();
    [[nodiscard]] std::list<BulletEntity>& GetBullets();
    [[nodiscard]] const std::list<BulletEntity>& GetBullets() const;

    int GetEnemyId() const;

private:
    CharacterEntity entity;
    sf::Sprite sprite;
    int damage{1};
    std::list<BulletEntity> bullets{};
    float shoot_cooldown{0};
};

struct Explosion {
    float lifetime{.25};
    sf::Sprite sprite;
    sf::Vector2f position;

    Explosion(const sf::Sprite &sprite, const sf::Vector2f& starting_pos);
};

#endif