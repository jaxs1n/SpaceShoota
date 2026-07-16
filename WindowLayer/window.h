//
// Created by jcksn on 7/12/2026.
//

#ifndef TESTGAME_WINDOWLAYER_H
#define TESTGAME_WINDOWLAYER_H

#include <SFML/Graphics/RenderWindow.hpp>

namespace GW {
    class Window {
    public:
        bool IsOpen() const;
        void PollEvents();
        void Close();
        void Display();

        void SetView(const sf::Vector2f& player_pos);
        void InitView();

        sf::RenderWindow window{sf::VideoMode({1000, 800}), "Space Game"};
    private:
        bool IsOpen_ = true;
        sf::View View_;
    };
}

#endif //TESTGAME_WINDOWLAYER_H