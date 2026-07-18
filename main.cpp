#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Init/Init.h"
#include "WindowLayer/window.h"
#include "WindowLayer/renderer.h"
#include "Collisions/Collisions.h"

bool spawnEnemy() {
    if (rand() % 3571 == 0){
        return true;
    }
    return false;
}

int main() {
    GW::Window window;
    sf::Clock clock;
    window.InitView();

    std::vector<Enemy> enemies{};

    const sf::Texture playerTexture{"player.png"};

    const sf::Texture backgroundTexture{"background.png"};

    sf::Sprite backgroundSprite{backgroundTexture};

    backgroundSprite.setScale(sf::Vector2f(20.f, 16.f));
    backgroundSprite.setPosition(sf::Vector2f(0.f, 0.f));

    Player player = InitializePlayer(playerTexture, {400, 400});

    sf::Vector2f closest_enemy_pos{};

    while (window.IsOpen()) {
        const float dt = clock.restart().asSeconds();
        window.PollEvents();
        float closestDistanceSquared = std::numeric_limits<float>::max();

        if (spawnEnemy()) {
            Enemy new_enemy = InitializeEnemies(playerTexture);
            enemies.push_back(new_enemy);
        }

        RD::Renderer::Clear(window.window);
        RD::Renderer::Draw(backgroundSprite, window.window);

        for (auto& enemy : enemies) {
            enemy.Update(dt, player.GetPosition());
            // RD::Renderer::Draw(enemy.GetShape(), window.window);
            RD::Renderer::Draw(enemy.GetSprite(), window.window);

            const sf::Vector2f difference = enemy.GetPosition() - player.GetPosition();

            const float distanceSquared =
                    difference.x * difference.x +
                    difference.y * difference.y;

            if (distanceSquared < closestDistanceSquared) {
                closestDistanceSquared = distanceSquared;
                closest_enemy_pos = enemy.GetPosition();
            }
        }

        player.Update(dt, closest_enemy_pos);

        for (auto it = player.GetBullets().begin(); it != player.GetBullets().end(); ) {
            bool hit = false;
            it->Update(dt);

            for (auto& enemy : enemies) {
                if (CircleToCircle(it->GetShape(), enemy.GetShape())) {
                    hit = true;
                }
            }
            if (hit) {
                it = player.GetBullets().erase(it);
            } else {
                RD::Renderer::Draw(it->GetShape(), window.window);
                ++it;
            }
        }

        RD::Renderer::Draw(player.GetSprite(), window.window);

        window.SetView(player.GetPosition());

        window.Display();
    }

    return 0;
}