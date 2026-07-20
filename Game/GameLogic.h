#ifndef TESTGAME_GAMELOGIC_H
#define TESTGAME_GAMELOGIC_H

#include <random>
#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>

#include "../Entities/Entities.h"
#include "../WindowLayer/window.h"

namespace Game {
    class GameLogic {
    public:
        GameLogic();
        void Run();

    private:
        static constexpr int MaxHealth{100};
        static constexpr float HealInterval{5.f};
        static constexpr float BulletCullDistance{2500.f};

        void Update(float dt);
        void Render();

        void SpawnEnemies(float dt);
        void UpdateEnemies(float dt);
        void UpdateEnemyBullets(Enemy& enemy, float dt);
        void UpdatePlayerBullets(float dt);
        void UpdateExplosions(float dt);
        void UpdateHealing(float dt);
        void RemoveDeadEnemies();
        void ResetAfterDeath();
        void UpdateHud();

        [[nodiscard]] bool IsBulletTooFar(const BulletEntity& bullet) const;
        void AddExplosion(const sf::Vector2f& position);
        static void CenterSpriteOrigin(sf::Sprite& sprite);

        GW::Window window_;
        sf::Clock frameClock_;

        sf::Texture playerTexture_{"player.png"};
        sf::Texture backgroundTexture_{"background.png"};
        sf::Texture explosionTexture_{"explosion.png"};

        sf::Sprite backgroundSprite_{backgroundTexture_};
        sf::Sprite bulletExplosionSprite_{explosionTexture_};

        Player player_;
        std::vector<Enemy> enemies_;
        std::vector<Explosion> explosions_;

        sf::Font font_{"arial.ttf"};
        sf::Text healthText_{font_};
        sf::Text scoreText_{font_};
        sf::Text highScoreText_{font_};

        int currentScore_{};
        int highScore_{};
        float healTimer_{};
        float spawnTimer_{};
        float nextSpawnDelay_{};

        std::mt19937 randomEngine_{std::random_device{}()};
        std::uniform_real_distribution<float> spawnDelayDistribution_{0.75f, 1.75f};
    };
}

#endif
