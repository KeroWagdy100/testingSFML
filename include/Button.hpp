#pragma once
#include <SFML/Graphics.hpp>
// #include <Calculator.hpp>


class Button {
public:
    // Enums
    enum Type {
        AC = 0, POWER,  MOD,   DIVIDE,
        SEVEN,  EIGHT,  NINE,  CROSS,
        FOUR,   FIVE,   SIX,   MINUS,
        ONE,    TWO,    THREE, PLUS,
        NON,    ZERO,   DOT,   EQUAL
    };

    enum Rank {
        MAIN = 0, SEC, NUM
    };

    enum State {
        DEF = 0, HOVER, PRESSED
    };
    ////////////////////////////////////
    
    // Constructor
    Button();
    ////////////////////////////////////

    void init(sf::RenderWindow* window, Button::Type type);
    
    void updatePosition();
    void updateColor();

    bool contains(const sf::Vector2i& point);
    inline bool isHovered() {return m_state == State::HOVER;}
    inline bool isPressed() {return m_state == State::PRESSED;}

    void hover();
    void press();
    void release();

    void draw();
    static std::string getString(Type buttonType);
    static sf::Vector2f toFloat(const sf::Vector2i iV);

    // STATIC DATA
    static const sf::Font font;
    static const float radius;
    // static const sf::Color MAIN_BG   ;  // Main Button Color
    // static const sf::Color SEC_BG    ;  // Secondary Button Color
    // static const sf::Color NUM_BG    ;  // Num Button Color
    // static const sf::Color TEXT_COLOR;  // Text Color
    ////////////////////////////////////
private:
    sf::RenderWindow*   m_window {nullptr};
    sf::CircleShape     m_circle{radius, 1000};
    sf::Text            m_text {font};
    bool                isInit {false};
    Rank                m_rank;
    Type                m_type;
    State               m_state;
    sf::Color           m_mainColor;
};