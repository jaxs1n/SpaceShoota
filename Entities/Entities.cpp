#include "Entities.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <utility>
#include "steering.h"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/Mouse.hpp"

// Reflective Shield Power Up
// Aimbot Power Up
// Super Speed Power Up
// Slow-Mo Power Up
// "Gold Rush" Power Up - More points, enemies become one tap -- maybe every 100 points?
// Juggernaut Enemy Type
// Boss
// Fix aspect ratio -- Resolution scaling & allow full screen
// Homing Missiles
// special attacks? -- RMB/Spacebar cooldown abilities?

namespace {
    int nextBulletId = 0;
}

namespace {
    int nextEnemyId = 0;
}

namespace {
    int next_entity_id = 0;
}

namespace {
    std::map<int, PlayerItem> id_to_playerItem = {
        {1, PlayerItem::Double_Shot},
        {2, PlayerItem::Cannon_Shot},
        {3, PlayerItem::Super_Speed},
        {4, PlayerItem::Slow_Mo}
    };
}

namespace
{
    sf::Texture PlayerBulletTexture{"bullet.png"};
    sf::Texture EnemyBulletTexture{"bullet2.png"};
    sf::Texture CannonBulletTexture{"cannon_ball.png"};

    sf::Sprite CannonBulletSprite{CannonBulletTexture};
    sf::Sprite PlayerBullet{PlayerBulletTexture};
    sf::Sprite EnemyBullet{EnemyBulletTexture};

    void CenterSpriteOrigin(sf::Sprite& sprite)
    {
        const sf::FloatRect bounds = sprite.getLocalBounds();

        sprite.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        });
    }
}
namespace {
    constexpr float attack_speed_0 = 0.f;
    constexpr float attack_speed_1 = 0.3f;
    constexpr float attack_speed_2 = 0.5f;
    constexpr float attack_speed_3 = 0.63;

    constexpr float armor_0 = 0.f;
    constexpr float armor_1 = 1.f;
    constexpr float armor_2 = 2.f;
    constexpr float armor_3 = 3.f;
}

namespace {
    sf::CircleShape bullet{5, 30};
    sf::CircleShape cannon_ball{15, 30};
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
    velocity *= movement_speed;
    SetPosition(position + velocity * dt);
}

void CharacterEntity::SetSpeed(const float speed_) {
    this->movement_speed = speed_;
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

void CharacterEntity::SetHealth(const int health_) {
    this->health = health_;
}

BulletEntity::BulletEntity(sf::CircleShape shape, const sf::Vector2f &starting_pos, const sf::Vector2f &starting_velo, const int damage, const float b_speed, sf::Sprite  sprite) :
damage{damage}, hitbox{std::move(shape)}, position{starting_pos}, velocity(starting_velo * b_speed), sprite{std::move(sprite)},Id{nextBulletId++} {
    hitbox.setPosition(position);
    hitbox.setOrigin({shape.getRadius(), shape.getRadius()});
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
entity{sf::CircleShape{40.f, 30},100, sf::Vector2f{starting_pos.x, starting_pos.y}}, sprite(std::move(sprite)) {
    CenterSpriteOrigin(this->sprite);
    this->sprite.setPosition({starting_pos.x, starting_pos.y});
}

void Player::Update(const float dt, const sf::RenderWindow& window) {
    shoot_cooldown_timer += dt;
    double_shot_cooldown_timer += dt;
    cannon_shot_cooldown_timer += dt;

    sf::Vector2f movementDirection{0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movementDirection.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movementDirection.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movementDirection.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movementDirection.x += 1.f;

    const sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && shoot_cooldown_timer >= attack_speed_1) {
        Shoot(Steering::PlayerDirectionToMouse(mousePosition, entity.GetPosition()));
        for (const auto& item : player_items) {
            if (item == PlayerItem::Double_Shot) {
                allow_double_shot = true;
                double_shot_cooldown_timer = 0.f;
                double_show_allow -= .1f;
            }
            if (item == PlayerItem::Cannon_Shot && cannon_shot_cooldown_timer >= cannon_shot_allow) {
                ShootCannon(Steering::PlayerDirectionToMouse(mousePosition, entity.GetPosition()));
                cannon_shot_cooldown_timer = 0.f;
                cannon_shot_allow -= 0.1f;
            }
            if (item == PlayerItem::Super_Speed) {
                entity.SetSpeed(tally_speed = 600.f);
            }
        }
        shoot_cooldown_timer = 0.f;
    }
    if (allow_double_shot && double_shot_cooldown_timer >= .05f) {
        Shoot(Steering::PlayerDirectionToMouse(mousePosition, entity.GetPosition()));
        double_shot_cooldown_timer = 0.f;
        allow_double_shot = false;
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

    entity.SetVelocity(movementDirection);
    entity.Update(dt);
    sprite.setPosition(entity.GetPosition());
}

void Player::AddItem(const int item_id) {

    const auto item_it = id_to_playerItem.find(item_id);

    if (item_it == id_to_playerItem.end()) {
        return;
    }

    const PlayerItem& item = item_it->second;

    for (const auto& local_item : player_items) {
        if (local_item == item) {
            return;
        }
    }

    player_items.push_back(item);
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
    bullets.emplace_back(bullet, entity.GetPosition(), closest_enemy_pos, damage, bullet_speed, PlayerBullet);
}
void Player::ShootCannon(const sf::Vector2f& closest_enemy_pos) {
    bullets.emplace_back(cannon_ball, entity.GetPosition(), closest_enemy_pos, 10, 700, CannonBulletSprite);
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

std::vector<PlayerItem>& Player::GetPlayerItems() {
    return player_items;
}

Enemy::Enemy(sf::Sprite sprite, const sf::Vector2f& starting_pos, const EnemyType enemy_type) :
    enemy_type(enemy_type),
    entity{sf::CircleShape{50.f, 100},
    0,
    starting_pos},
    sprite(std::move(sprite))
{
    CenterSpriteOrigin(this->sprite);
    this->sprite.setPosition({starting_pos.x, starting_pos.y});

    switch (enemy_type) {
        case EnemyType::Normal:
            entity.SetHealth(5);
            damage = 1;
            speed = 350.f;
            shoot_cooldown = attack_speed_2;
            follow_distance = 100;
            break;

        case EnemyType::Tank:
            entity.SetHealth(10);
            damage = 1;
            speed = 250.f;
            shoot_cooldown = attack_speed_3;
            follow_distance = 100;
            break;

        case EnemyType::Sniper:
            entity.SetHealth(3);
            damage = 3;
            speed = 250.f;
            shoot_cooldown = attack_speed_3;
            follow_distance = 300;
            bullet_speed = 2500.f;
            break;

        case EnemyType::Stealth:
            entity.SetHealth(2);
            damage = 1;
            speed = 450.f;
            shoot_cooldown = attack_speed_1;
            follow_distance = 50;
            break;
    }
}

void Enemy::Update(const float dt, const sf::Vector2f& player_pos) {
    shoot_cooldown_timer += dt;
    const sf::Vector2f direction = Steering::EnemyToPlayer(entity.GetPosition(), player_pos);

    if (Steering::StopEnemyMovement(player_pos, entity.GetPosition(), follow_distance)) {
        entity.SetVelocity(direction);
        entity.Update(dt);
        sprite.setPosition({entity.GetPosition().x, entity.GetPosition().y});
    }
}
void Enemy::SetVelocity(const sf::Vector2f velocity) {
    entity.SetVelocity(velocity);
}
void Enemy::SetAcceleration(const sf::Vector2f acceleration) {
    entity.SetAcceleration(acceleration);
}

void Enemy::Shoot(const sf::Vector2f &player_position) {
    const sf::Vector2f direction = Steering::EnemyToPlayer(entity.GetPosition(), player_position);
    if (shoot_cooldown_timer > shoot_cooldown) {
        sf::CircleShape bullet_{5, 30};
        bullets.emplace_back(bullet_, entity.GetPosition(), direction, damage, bullet_speed, EnemyBullet);
        shoot_cooldown_timer = 0.f;
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

ItemEntity::ItemEntity(const sf::CircleShape &shape, const sf::Vector2f &spawn_position, const sf::Sprite &sprite, const int item_id) :
    Entity_Id(next_entity_id++),
    Item_Id(item_id),
    hitbox(shape),
    position(spawn_position),
    sprite(sprite)
{
    this->sprite.setScale({.3, .3});
    this->sprite.setPosition(spawn_position);
    const sf::FloatRect bounds = this->sprite.getLocalBounds();
    this->sprite.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});

    this->hitbox.setPosition(spawn_position);
    this->hitbox.setOrigin({shape.getRadius(), shape.getRadius()});
}

const sf::Vector2f &ItemEntity::GetPosition() const {
    return position;
}

int ItemEntity::GetItemId() const {
    return Item_Id;
}

int ItemEntity::GetEntityId() const {
    return Entity_Id;
}

float ItemEntity::GetTimeAlive() const {
    return time_alive;
}

const sf::CircleShape &ItemEntity::GetHitbox() const {
    return hitbox;
}

const sf::Sprite &ItemEntity::GetSprite() const {
    return sprite;
}

bool ItemEntity::GetPickupState() const {
    return picked_up;
}

void ItemEntity::Update(const float dt) {
    time_alive += dt;
}

void ItemEntity::Pickup() {
    picked_up = true;
}

Explosion::Explosion(const sf::Sprite &sprite, const sf::Vector2f &starting_pos) : sprite(sprite), position(starting_pos) {
    this->sprite.setPosition(position);
}
