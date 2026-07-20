#include "GameLogic.h"

#include <algorithm>
#include <cmath>
#include <string>

#include "../Collisions/Collisions.h"
#include "../Entities/Spawn.h"
#include "../Init/Init.h"
#include "../WindowLayer/renderer.h"

namespace {
    constexpr sf::Vector2f PlayerStart{1000.f, 1000.f};
}

Game::GameLogic::GameLogic()
    : player_{Spawn::InitializePlayer(playerTexture_, PlayerStart)} {
    nextSpawnDelay_ = spawnDelayDistribution_(randomEngine_);
    window_.InitView();

    backgroundSprite_.setScale({20.f, 16.f});
    backgroundSprite_.setPosition({0.f, 0.f});

    bulletExplosionSprite_.setScale({0.15f, 0.15f});
    CenterSpriteOrigin(bulletExplosionSprite_);

    healthText_.setCharacterSize(30);
    scoreText_.setCharacterSize(30);
    highScoreText_.setCharacterSize(30);
}

void Game::GameLogic::Run() {
    while (window_.IsOpen()) {
        const float dt = std::min(frameClock_.restart().asSeconds(), 0.1f);
        window_.PollEvents();
        if (!window_.IsOpen()) {
            break;
        }

        Update(dt);
        Render();
    }
}

void Game::GameLogic::Update(const float dt) {
    SpawnEnemies(dt);
    UpdateEnemies(dt);

    player_.Update(dt, window_.window);
    UpdatePlayerBullets(dt);

    RemoveDeadEnemies();
    UpdateExplosions(dt);
    UpdateHealing(dt);

    if (player_.GetEntity().GetHealth() <= 0) {
        ResetAfterDeath();
    }

    UpdateHud();
    window_.SetView(player_.GetPosition());
}

void Game::GameLogic::Render() {
    RD::Renderer::Clear(window_.window);
    RD::Renderer::Draw(backgroundSprite_, window_.window);

    for (const auto& enemy : enemies_) {
        RD::Renderer::Draw(enemy.GetSprite(), window_.window);

        for (const auto& bullet : enemy.GetBullets()) {
            RD::Renderer::Draw(bullet.GetSprite(), window_.window);
        }
    }

    for (const auto& bullet : player_.GetBullets()) {
        RD::Renderer::Draw(bullet.GetSprite(), window_.window);
    }

    for (const auto& explosion : explosions_) {
        RD::Renderer::Draw(explosion.sprite, window_.window);
    }

    RD::Renderer::Draw(player_.GetSprite(), window_.window);
    RD::Renderer::Draw(highScoreText_, window_.window);
    RD::Renderer::Draw(scoreText_, window_.window);
    RD::Renderer::Draw(healthText_, window_.window);

    window_.Display();
}

void Game::GameLogic::SpawnEnemies(const float dt) {
    spawnTimer_ += dt;

    if (spawnTimer_ < nextSpawnDelay_) {
        return;
    }

    enemies_.push_back(
        Spawn::InitializeEnemy(
            playerTexture_,
            EnemySpawnLocation(player_.GetPosition()),
            RandomEnemyType()
        )
    );

    spawnTimer_ = 0.f;
    nextSpawnDelay_ = spawnDelayDistribution_(randomEngine_);
}

void Game::GameLogic::UpdateEnemies(const float dt) {
    for (auto& enemy : enemies_) {
        enemy.PointToPlayer(player_.GetPosition());
        enemy.Shoot(player_.GetPosition());
        enemy.Update(dt, player_.GetPosition());
        UpdateEnemyBullets(enemy, dt);
    }
}

void Game::GameLogic::UpdateEnemyBullets(Enemy& enemy, const float dt) {
    auto& bullets = enemy.GetBullets();

    for (auto bullet = bullets.begin(); bullet != bullets.end();) {
        bullet->Update(dt);

        if (CircleToCircle(bullet->GetShape(), player_.GetShape())) {
            player_.TakeDamage();
            AddExplosion(player_.GetPosition());
            bullet = bullets.erase(bullet);
            continue;
        }

        if (IsBulletTooFar(*bullet)) {
            bullet = bullets.erase(bullet);
            continue;
        }

        ++bullet;
    }
}

void Game::GameLogic::UpdatePlayerBullets(const float dt) {
    auto& bullets = player_.GetBullets();

    for (auto bullet = bullets.begin(); bullet != bullets.end();) {
        bullet->Update(dt);
        bool hitEnemy = false;

        for (auto& enemy : enemies_) {
            if (!CircleToCircle(bullet->GetShape(), enemy.GetShape())) {
                continue;
            }

            enemy.GetEntity().TakeDamage(player_.GetDamage());
            AddExplosion(enemy.GetPosition());
            hitEnemy = true;
            break;
        }

        if (hitEnemy || IsBulletTooFar(*bullet)) {
            bullet = bullets.erase(bullet);
        } else {
            ++bullet;
        }
    }
}

void Game::GameLogic::UpdateExplosions(const float dt) {
    for (auto explosion = explosions_.begin(); explosion != explosions_.end();) {
        explosion->lifetime -= dt;

        if (explosion->lifetime <= 0.f) {
            explosion = explosions_.erase(explosion);
        } else {
            ++explosion;
        }
    }
}

void Game::GameLogic::UpdateHealing(const float dt) {
    healTimer_ += dt;

    if (healTimer_ < HealInterval) {
        return;
    }

    auto& entity = player_.GetEntity();
    entity.SetHealth(std::min(entity.GetHealth() + 1, MaxHealth));
    healTimer_ = 0.f;
}

void Game::GameLogic::RemoveDeadEnemies() {
    const auto oldSize = enemies_.size();

    std::erase_if(enemies_, [](Enemy& enemy) {
        return enemy.GetEntity().GetHealth() <= 0;
    });

    currentScore_ += static_cast<int>(oldSize - enemies_.size());
}

void Game::GameLogic::ResetAfterDeath() {
    highScore_ = std::max(highScore_, currentScore_);
    currentScore_ = 0;
    healTimer_ = 0.f;

    enemies_.clear();
    explosions_.clear();
    player_.GetBullets().clear();

    player_.SetPosition(PlayerStart);
    player_.GetEntity().SetHealth(MaxHealth);
}

void Game::GameLogic::UpdateHud() {
    scoreText_.setString("Score: " + std::to_string(currentScore_));
    highScoreText_.setString("High Score: " + std::to_string(highScore_));
    healthText_.setString("Health: " + std::to_string(player_.GetEntity().GetHealth()));

    const sf::Vector2f topLeft = player_.GetPosition() + sf::Vector2f{-500.f, -450.f};
    scoreText_.setPosition(topLeft);
    highScoreText_.setPosition(topLeft + sf::Vector2f{0.f, 40.f});
    healthText_.setPosition(topLeft + sf::Vector2f{0.f, 80.f});
}

bool Game::GameLogic::IsBulletTooFar(const BulletEntity& bullet) const {
    const sf::Vector2f offset = bullet.GetPosition() - player_.GetPosition();
    return offset.x * offset.x + offset.y * offset.y >
           BulletCullDistance * BulletCullDistance;
}

void Game::GameLogic::AddExplosion(const sf::Vector2f& position) {
    explosions_.emplace_back(bulletExplosionSprite_, position);
}

void Game::GameLogic::CenterSpriteOrigin(sf::Sprite& sprite) {
    const sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });
}
