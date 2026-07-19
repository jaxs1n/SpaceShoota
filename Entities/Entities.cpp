#include "Entities.h"

#include <algorithm>
#include <cmath>
#include <utility>
#include "steering.h"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/Mouse.hpp"

namespace {
    int nextBulletId = 0;
}

namespace {
    int nextEnemyId = 0;
}
namespace
{
    sf::Texture PlayerBulletTexture{"bullet.png"};
    sf::Texture EnemyBulletTexture{"bullet2.png"};

    sf::Sprite playerB{PlayerBulletTexture};
    sf::Sprite enemyB{EnemyBulletTexture};

    void CenterSpriteOrigin(sf::Sprite& sprite)
    {
        const sf::FloatRect bounds = sprite.getLocalBounds();

        sprite.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        });
    }
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

void CharacterEntity::TakeDamage(const int damage) {
    health = health - damage;

}
void CharacterEntity::TakeDamage() {
    this->health--;
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

void CharacterEntity::SetHealth(const int health) {
    this->health = health;
}

BulletEntity::BulletEntity(sf::CircleShape shape, const sf::Vector2f &starting_pos, const sf::Vector2f &starting_velo, const int damage, sf::Sprite  sprite) :
damage{damage}, hitbox{std::move(shape)}, position{starting_pos}, sprite{std::move(sprite)}, Id{nextBulletId++} {
    hitbox.setPosition(position);
    hitbox.setOrigin({shape.getRadius(), shape.getRadius()});
    velocity = bullet_speed * starting_velo;

    this->sprite.setScale({.1, .1});

    const sf::FloatRect bounds = this->sprite.getLocalBounds();

    this->sprite.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    this->sprite.setPosition(position);

    const float angle = std::atan2(velocity.y, velocity.x);
    this->sprite.setRotation(sf::radians(angle));
}

void BulletEntity::Update(const float dt) {
    position += velocity * dt;
    hitbox.setPosition(position);
    sprite.setPosition(position);
}

const sf::CircleShape& BulletEntity::GetShape() const {
    return hitbox;
}

sf::Vector2f BulletEntity::GetPosition() const {
    return position;
}

const sf::Sprite &BulletEntity::GetSprite() const {
    return sprite;
}

int BulletEntity::GetId() const {
    return Id;
}

Player::Player(sf::Sprite sprite, const sf::Vector2f& starting_pos) :
entity{sf::CircleShape{40.f, 30},5, sf::Vector2f{starting_pos.x, starting_pos.y}}, sprite(std::move(sprite)) {
    CenterSpriteOrigin(this->sprite);
    this->sprite.setPosition({starting_pos.x, starting_pos.y});
}

void Player::Update(const float dt, const sf::RenderWindow& window) {
    shoot_cooldown += dt;

    sf::Vector2f movementDirection{0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movementDirection.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movementDirection.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movementDirection.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movementDirection.x += 1.f;

    const sf::Vector2f mousePosition =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && shoot_cooldown >= 0.5f) {
        Shoot(Steering::PlayerDirectionToMouse(mousePosition, entity.GetPosition()));
        shoot_cooldown = 0.f;
    }

    sprite.setRotation(sf::degrees(
        Steering::PlayerPointToMouse(mousePosition, entity.GetPosition())
    ));

    const float movementLength = std::sqrt(
        movementDirection.x * movementDirection.x +
        movementDirection.y * movementDirection.y
    );

    if (movementLength > 0.f) {
        movementDirection /= movementLength;
    }

    constexpr float movementSpeed = 450.f;
    entity.SetVelocity(movementDirection * movementSpeed);
    entity.Update(dt);
    sprite.setPosition(entity.GetPosition());
}

void Player::SetVelocity(const sf::Vector2f velocity) {
    entity.SetVelocity(velocity);
}

void Player::SetAcceleration(const sf::Vector2f acceleration) {
    entity.SetAcceleration(acceleration);
}

void Player::SetPosition(const sf::Vector2f position) {
    entity.SetPosition(position);
}

void Player::TakeDamage() {
    entity.TakeDamage();
}

void Player::Shoot(const sf::Vector2f& closest_enemy_pos) {
    sf::CircleShape bullet{5, 30};
    bullets.emplace_back(bullet, entity.GetPosition(), closest_enemy_pos, damage, playerB);
}

void Player::RemoveBullet(const int bulletId) {
    std::erase_if(bullets, [bulletId](const BulletEntity &b) {
        return b.GetId() == bulletId;
    });
}

void Player::SetDamage(const int dmg) {
    this->damage += dmg;
}

sf::Vector2f Player::GetPosition() const {
    return entity.GetPosition();
}

const sf::CircleShape& Player::GetShape() const {
    return entity.GetShape();
}

CharacterEntity &Player::GetEntity() {
    return entity;
}

const sf::Sprite& Player::GetSprite() const {
    return sprite;
}

int Player::GetDamage() const {
    return damage;
}

std::list<BulletEntity>& Player::GetBullets() {
    return bullets;
}

const std::list<BulletEntity>& Player::GetBullets() const {
    return bullets;
}

Enemy::Enemy(sf::Sprite sprite, const sf::Vector2f& spawn_position) : entity{sf::CircleShape{50.f, 100}, 5, {200, 200}}, sprite(std::move(sprite)) {
    this->entity.SetPosition(spawn_position);
    CenterSpriteOrigin(this->sprite);
    this->sprite.setPosition({spawn_position.x, spawn_position.y});
}

void Enemy::Update(const float dt, const sf::Vector2f& player_pos) {
    shoot_cooldown += dt;
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

void Enemy::Shoot(const sf::Vector2f &player_position) {
    const sf::Vector2f direction = Steering::EnemyToPlayer(entity.GetPosition(), player_position);
    if (shoot_cooldown > .5) {
        sf::CircleShape bullet{5, 30};
        bullets.emplace_back(bullet, entity.GetPosition(), direction, damage, enemyB);
        shoot_cooldown = 0.f;
    }
}

void Enemy::PointToPlayer(const sf::Vector2f &player_position) {
    const float theta = Steering::PlayerPointToEnemy(entity.GetPosition(), player_position);
    sprite.setRotation(sf::degrees(theta));
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

std::list<BulletEntity>& Enemy::GetBullets() {
    return bullets;
}

const std::list<BulletEntity>& Enemy::GetBullets() const {
    return bullets;
}

CharacterEntity& Enemy::GetEntity() {
    return entity;
}

Explosion::Explosion(const sf::Sprite &sprite, const sf::Vector2f &starting_pos) : sprite(sprite), position(starting_pos) {
    this->sprite.setPosition(position);
}
