#include "Entities.h"

#include <algorithm>
#include <utility>
#include "steering.h"
#include "SFML/Window/Keyboard.hpp"
#include <cmath>
#include <iostream>

namespace {
    int nextBulletId = 0;
}

namespace {
    int nextEnemyId = 0;
}
CharacterEntity::CharacterEntity(sf::CircleShape shape, const int health, const sf::Vector2f position)
    : health{health},
      Id{nextEnemyId++},
      position{position},
      shape{std::move(shape),
      }
{
    this->shape.setOrigin({shape.getRadius(), shape.getRadius()});
    this->shape.setPosition(position);
}

void CharacterEntity::Update(const float dt) {
    velocity += acceleration * dt;
    SetPosition(position + velocity * dt);
}

void CharacterEntity::Die() {
    alive = false;
}

void CharacterEntity::TakeDamage(const int damage) {
    health = std::max(0, health - damage);

    if (health <= 0) {
        Die();
    }
}

bool CharacterEntity::IsAlive() const {
    return alive;
}

int CharacterEntity::GetHealth() const {
    return health;
}

int CharacterEntity::GetId() const {
    return Id;
}

sf::Vector2f CharacterEntity::GetPosition() const {
    return position;
}

sf::Vector2f CharacterEntity::GetVelocity() const {
    return velocity;
}

sf::Vector2f CharacterEntity::GetAcceleration() const {
    return acceleration;
}

const sf::CircleShape& CharacterEntity::GetShape() const {
    return shape;
}

sf::CircleShape& CharacterEntity::GetShape() {
    return shape;
}

void CharacterEntity::SetPosition(const sf::Vector2f newPosition) {
    position = newPosition;
    shape.setPosition(position);
}

void CharacterEntity::SetVelocity(const sf::Vector2f newVelocity) {
    velocity = newVelocity;
}

void CharacterEntity::SetAcceleration(const sf::Vector2f newAcceleration) {
    acceleration = newAcceleration;
}

BulletEntity::BulletEntity(sf::CircleShape shape, const sf::Vector2f &starting_pos, const sf::Vector2f &starting_velo, const int damage) :
damage{damage}, hitbox{std::move(shape)}, position{starting_pos}, Id{nextBulletId++} {
    hitbox.setPosition(position);
    hitbox.setOrigin({shape.getRadius(), shape.getRadius()});
    velocity = bullet_speed * starting_velo;
}

void BulletEntity::Update(const float dt) {
    position += velocity * dt;
    hitbox.setPosition(position);
}

const sf::CircleShape& BulletEntity::GetShape() const {
    return hitbox;
}

int BulletEntity::GetId() const {
    return Id;
}

Player::Player(sf::Sprite sprite, const sf::Vector2f& starting_pos) :
entity{sf::CircleShape{40.f, 30},100, sf::Vector2f{starting_pos.x, starting_pos.y}}, sprite(std::move(sprite)) {
    this->sprite.setPosition({starting_pos.x, starting_pos.y});
}

void Player::Update(const float dt, const sf::Vector2f enemy_pos) {
    sf::Vector2f direction{0.f, 0.f};

    const float closest_enemy_theta = Steering::PlayerPointToEnemy(entity.GetPosition(), enemy_pos);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        direction.y -= 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        direction.y += 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        direction.x -= 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        direction.x += 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        Player::Shoot(enemy_pos);
    }

    sprite.setRotation(sf::degrees(closest_enemy_theta));

    const float length = std::sqrt(
            direction.x * direction.x +
            direction.y * direction.y
        );
    if (length > 0.f) {
        direction /= length;
    }

    constexpr float movementSpeed = 400.f;

    entity.SetVelocity(direction * movementSpeed);

    entity.Update(dt);
    sprite.setPosition({entity.GetPosition().x, entity.GetPosition().y});
}

void Player::SetVelocity(const sf::Vector2f velocity) {
    entity.SetVelocity(velocity);
}

void Player::SetAcceleration(const sf::Vector2f acceleration) {
    entity.SetAcceleration(acceleration);
}

void Player::Shoot(const sf::Vector2f& closest_enemy_pos) {
    sf::CircleShape bullet{2, 30};
    bullets.emplace_back(sf::CircleShape{2, 30}, entity.GetPosition(), Steering::EnemyToPlayer(entity.GetPosition(), closest_enemy_pos), damage);
}

void Player::RemoveBullet(const int bulletId) {
    std::erase_if(bullets, [bulletId](const BulletEntity &b) {
        return b.GetId() == bulletId;
    });
}

sf::Vector2f Player::GetPosition() const {
    return entity.GetPosition();
}

const sf::CircleShape& Player::GetShape() const {
    return entity.GetShape();
}

const sf::Sprite& Player::GetSprite() const {
    return sprite;
}

std::list<BulletEntity>& Player::GetBullets() {
    return bullets;
};

Enemy::Enemy(sf::Sprite  sprite) : entity{sf::CircleShape{50.f, 100}, 10, {200, 200}}, sprite(std::move(sprite)) {}

void Enemy::Update(const float dt, const sf::Vector2f& player_pos) {
    const sf::Vector2f direction = Steering::EnemyToPlayer(entity.GetPosition(), player_pos);

    constexpr float speed = 350.f;

    entity.SetVelocity(direction * speed);
    entity.Update(dt);
    sprite.setPosition({entity.GetPosition().x, entity.GetPosition().y});
}
void Enemy::SetVelocity(const sf::Vector2f velocity) {
    entity.SetVelocity(velocity);
}
void Enemy::SetAcceleration(const sf::Vector2f acceleration) {
    entity.SetAcceleration(acceleration);
}
sf::Vector2f Enemy::GetPosition() const {
    return entity.GetPosition();
}
const sf::CircleShape &Enemy::GetShape() const {
    return entity.GetShape();
}

const sf::Sprite &Enemy::GetSprite() const {
    return sprite;
}
int Enemy::GetEnemyId() const {
    return entity.GetId();
}