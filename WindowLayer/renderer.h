//
// Created by jcksn on 7/12/2026.
//

#ifndef TESTGAME_RENDERER_H
#define TESTGAME_RENDERER_H

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

namespace RD {
    class Renderer {
    public:
        static void Draw(const sf::CircleShape& circle, sf::RenderWindow& window);
        static void Draw(const sf::Drawable& drawable, sf::RenderWindow& window);
        static void Draw(const sf::RectangleShape& rectangle, sf::RenderWindow& window);
        static void Clear(sf::RenderWindow& window);
    private:
    };
}

#endif //TESTGAME_RENDERER_H