#ifndef TESTGAME_ENTITIES_H
#define TESTGAME_ENTITIES_H

#include <list>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"

// Reflective Shield Power Up
// Aimbot Power Up
// Super Speed Power Up
// Slow-Mo Power Up
// "Gold Rush" Power Up - More points, enemies become one tap -- maybe every 100 points?

enum class EnemyType {
    Normal,
    Tank,
    Stealth,
    Sniper
};

enum class PlayerItem {
    Triple_Shot,
    Double_Shot,
    Cannon_Shot,
    Reflective_Shield,
    Aimbot,
    Super_Speed,
    Slow_Mo
};

class CharacterEntity {
public:
    CharacterEntity(sf::CircleShape shape, int health, sf::Vector2f position);\

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
    void SetHealth(int health_);
    void SetSpeed(float speed_);
    void TakeDamage();
    void Update(float dt);
    void TakeDamage(int damage);

private:
    int health{};
    int Id{};
    float movement_speed{450};
    float shoot_cooldown{.5};

    sf::Vector2f position{};
    sf::Vector2f velocity{};
    sf::Vector2f acceleration{};

    sf::CircleShape shape;
};

class BulletEntity {
public:
    BulletEntity(sf::CircleShape shape, const sf::Vector2f &starting_pos, const sf::Vector2f &starting_velo, int damage, float b_speed, sf::Sprite sprite);

    void Update(float dt);

    const sf::CircleShape& GetShape() const;
    const sf::Sprite& GetSprite() const;
    sf::Vector2f GetPosition() const;
    int GetId() const;

private:
    const int Id{};
    int damage{};
    sf::CircleShape hitbox;
    sf::Vector2f position{};
    sf::Vector2f velocity{};
    sf::Sprite sprite;
};

class ItemEntity {
public:
    ItemEntity(const sf::CircleShape &shape, const sf::Vector2f &spawn_position, const sf::Sprite &sprite, int item_id);

    void Update(float dt);
    void Pickup();

    [[nodiscard]] int GetEntityId() const;
    [[nodiscard]] int GetItemId() const;
    [[nodiscard]] float GetTimeAlive() const;
    [[nodiscard]] const sf::CircleShape& GetHitbox() const;
    [[nodiscard]] const sf::Vector2f& GetPosition() const;
    [[nodiscard]] const sf::Sprite& GetSprite() const;
    [[nodiscard]] bool GetPickupState() const;

private:
    int Entity_Id;
    int Item_Id;
    float time_alive{0.f};
    bool picked_up{false};
    sf::CircleShape hitbox;
    sf::Vector2f position{};
    sf::Sprite sprite;
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
    void ShootCannon(const sf::Vector2f& closest_enemy_pos);
    void RemoveBullet(int bulletId);
    void AddItem(int item_id);

    [[nodiscard]] sf::Vector2f GetPosition() const;
    [[nodiscard]] const sf::CircleShape& GetShape() const;
    [[nodiscard]] const sf::Sprite& GetSprite() const;
    [[nodiscard]] int GetDamage() const;
    [[nodiscard]] CharacterEntity& GetEntity();
    [[nodiscard]] std::list<BulletEntity>& GetBullets();
    [[nodiscard]] const std::list<BulletEntity>& GetBullets() const;
    [[nodiscard]] std::vector<PlayerItem>& GetPlayerItems();

private:
    int damage{2};
    float shoot_cooldown_timer{0.f};
    float double_shot_cooldown_timer{0.f};
    float cannon_shot_cooldown_timer{0.f};
    float cannon_shot_allow{.9f};
    float double_show_allow{.6f};
    float bullet_speed{1400.f};
    float tally_speed{450};
    bool allow_double_shot{false};
    sf::Sprite sprite;
    CharacterEntity entity;
    std::vector<PlayerItem> player_items{};
    std::list<BulletEntity> bullets{};
};

class Enemy {
public:
    explicit Enemy(sf::Sprite sprite, const sf::Vector2f& starting_pos, EnemyType enemy_type);

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
    int follow_distance{50};
    int damage{1};
    float bullet_speed{1400.f};
    float speed{0.f};
    float shoot_cooldown{0.5f};
    float shoot_cooldown_timer{0};
    sf::Sprite sprite;
    EnemyType enemy_type{EnemyType::Normal};
    CharacterEntity entity;
    std::list<BulletEntity> bullets{};
};

struct Explosion {
    float lifetime{.25};
    sf::Sprite sprite;
    sf::Vector2f position;

    Explosion(const sf::Sprite &sprite, const sf::Vector2f& starting_pos);
};

#endif