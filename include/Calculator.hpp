#pragma once
#include <SFML/Graphics.hpp>
#include <Button.hpp>
#include <LinkedList.hpp>
#include <cmath>
#include <array>
#include <iostream>

class Button;

class Calculator {
public:
    Calculator(sf::RenderWindow* window);

    void init();
    void updateAndRender();

    ////////////////////////////////////
    // Event Handling
    void mouseMoved(const sf::Vector2i& pos);
    void mousePressed(const sf::Vector2i& pos);
    void buttonPressed(char32_t c);
    void mouseReleased(const sf::Vector2i& pos);
    ////////////////////////////////////
    // Static
    static const sf::Font font;
    static const uint16_t nROWS {5};
    static const uint16_t nCOLS {4};
    // static const sf::Color BG_COLOR; // Calculator color
    static const float hMargin;        // horizontal margin
    static const float vMargin;        // vertical margin
    static const float bottomMargin;
    static const sf::Vector2u windowSize;
    static sf::Vector2f getPosOfButton(uint16_t i, uint16_t j);
    static sf::Vector2f round(const sf::Vector2f vector);
    static bool isNum(Button::Type typ);
    static bool isOperator(Button::Type typ);
    static bool isSpace(const sf::Keyboard::Key& key);
    static Button::Type getType(char32_t c);
    ////////////////////////////////////

private:

    ///////////////////////////////////// 
    // Member Data 
    sf::RenderWindow* m_window;
    Button buttons[nROWS][nCOLS];
    sf::Text          m_input {font};
    LinkedList<double> operands;
    LinkedList<Button::Type> operators;    
    std::string currOperand {""};
    Button::Type          lastType {Button::Type::NON};
};
