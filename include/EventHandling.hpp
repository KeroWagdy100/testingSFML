#pragma once
#include <SFML/Graphics.hpp>
#include <Calculator.hpp>

namespace {
    template <class T>
    std::string vec2ToString(const sf::Vector2<T>& vec)
    {
        return "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y);
    }
} // namepsace

class EventHandler {
public:
    EventHandler(sf::RenderWindow* window, Calculator* calculator):
        m_window(window), m_calculator(calculator)
    {
        m_log.setFillColor(sf::Color::White);
        m_log.setPosition({10.f, 10.f});
        m_log.setCharacterSize(14u);
        m_text.setFillColor(sf::Color::White);
        m_text.setPosition({10.f, 40.f});
        m_text.setCharacterSize(14u);
    }
    ///////////////////////////////////
    void handleEvents()
    {
        while (const auto event = m_window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                // CLOSING APP
                m_window->close();
                return;
            }

            if (const auto& text = event->getIf<sf::Event::TextEntered>())
            {
                auto charEntered = text->unicode;
                m_log.setString("Event: Text Entered [" + sf::String(charEntered) + "]");
                m_text.setString(m_text.getString() + charEntered);
                m_calculator->buttonPressed(charEntered);
            }

            else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                m_log.setString("Event: Key Pressed [" + sf::Keyboard::getDescription(key->scancode) + "]");
                // key->
                // switch ()
            }

            else if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
            {
                m_log.setString("Event: Mouse Moved " + vec2ToString(mouse->position));
                m_calculator->mousePressed(mouse->position);
            }

            else if (const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>())
            {
                m_log.setString("Event: Mouse Moved " + vec2ToString(mouse->position));
                m_calculator->mouseReleased(mouse->position);
            }

            else if (const auto* mouse = event->getIf<sf::Event::MouseMoved>())
            {
                m_log.setString("Event: Mouse Moved " + vec2ToString(mouse->position));
                m_calculator->mouseMoved(mouse->position);
            }


        }
    }

    ///////////////////////////////////
    inline void drawLog()
    {
        m_window->draw(m_text);
        m_window->draw(m_log);
    }
    
private:
    sf::RenderWindow* m_window; 
    Calculator* m_calculator; 
    sf::Font m_font {"res/fonts/Roboto/Roboto-Regular.ttf"}; // TODO: FILENAME NEEDED
    sf::Text m_log {m_font, ""};
    sf::Text m_text {m_font, "Text Stream: "};
};