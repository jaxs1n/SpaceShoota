//
// Created by jcksn on 7/12/2026.
//

#include "window.h"

bool GW::Window::IsOpen() const {
    return IsOpen_;
}

void GW::Window::Close() {
    window.close();
}

void GW::Window::PollEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

void GW::Window::SetView(const sf::Vector2f& player_pos) {
    View_.setCenter({player_pos.x, player_pos.y});
    window.setView(View_);
}

void GW::Window::InitView() {
    View_.setSize({1000.f, 1000.f});
    View_.setCenter({400.f, 400.f});

    window.setView(View_);
}

void GW::Window::Display() {
    window.display();
}
