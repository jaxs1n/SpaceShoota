//
// Created by jcksn on 7/12/2026.
//

#include "renderer.h"

void RD::Renderer::Draw(const sf::CircleShape &circle, sf::RenderWindow& window) {
    window.draw(circle);
}
void RD::Renderer::Draw(const sf::RectangleShape &rectangle, sf::RenderWindow& window) {
    window.draw(rectangle);
}
void RD::Renderer::Draw(const sf::Drawable &drawable, sf::RenderWindow& window) {
    window.draw(drawable);
}
void RD::Renderer::Clear(sf::RenderWindow& window) {
    window.clear();
}
