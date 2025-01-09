#include <Button.hpp>
#include <Calculator.hpp>
#include <Colors.hpp>


// template <class T>
// sf::Vector<T> operator+(sf::Vector<T>);

// Constructor
Button::Button(): m_text(Calculator::font) {}
////////////////////////////////////

void Button::init(sf::RenderWindow* window, Button::Type type) {
    isInit = true;        
    m_window = window;
    m_type = type;
    m_state = State::DEF;
    
    // setting rank & color
    sf::Color color;
    if ((m_type+1) % 4 == 0){
        m_rank = Rank::MAIN;
    }
    else if (m_type >= 0 && m_type <= 2) {
        m_rank = Rank::SEC;
    }
    else {
        m_rank = Rank::NUM;
    }
    updateColor();
    // --------------------------
    m_text.setString(getString(m_type));
    
    updatePosition();
}

void Button::updatePosition()
{
    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setOrigin(textBounds.getCenter());

    uint16_t i = m_type / Calculator::nCOLS;
    uint16_t j = m_type % Calculator::nCOLS;
    sf::Vector2f pos = Calculator::getPosOfButton(i, j);

    m_circle.setPosition(pos);

    sf::Vector2f circleCenter = m_circle.getPosition() + sf::Vector2f(m_circle.getRadius(), m_circle.getRadius());

    m_text.setPosition(circleCenter);
    // Optional fine-tuning (use this if needed for your specific font):
    // m_text.move({-10.f, -15.f}); // Fine-tune vertical centering
}

bool Button::contains(const sf::Vector2i& point) { 
    sf::Vector2f point2f = toFloat(point);
    return m_circle.getGlobalBounds().contains(point2f);
}

void Button::hover() {
    m_state = State::HOVER;
    updateColor();
}

void Button::press() {
    m_state = State::PRESSED;
    updateColor();
}

void Button::release() {
    m_state = State::DEF; 
    updateColor();
}

void Button::updateColor() {
    sf::Color defColor {m_rank == MAIN ? Color::mainButton : m_rank == SEC ? Color::secButton : Color::numButton};
    if (m_state == State::DEF)
        m_circle.setFillColor(defColor);
    else if (m_state == State::HOVER)
        m_circle.setFillColor(defColor + Color::hoverShift);
    else if (m_state == State::PRESSED)
        m_circle.setFillColor(defColor * Color::pressedShift);
}

void Button::draw() {
    m_window->draw(m_circle);
    m_window->draw(m_text);
}

std::string Button::getString(Type buttonType) {
    switch (buttonType)
    {
        case AC:        return "C";
        case POWER:     return "^";
        case MOD:       return "%";
        case DIVIDE:    return "/";
        
        case SEVEN:     return "7";
        case EIGHT:     return "8";
        case NINE:      return "9";
        case CROSS:     return "X";

        case FOUR:      return "4";
        case FIVE:      return "5";
        case SIX:       return "6";
        case MINUS:     return "-";

        case ONE:       return "1";
        case TWO:       return "2";
        case THREE:     return "3";
        case PLUS:      return "+";

        case Type::NON:       return  "";
        case ZERO:      return "0";
        case DOT:       return ".";
        case EQUAL:     return "=";

        default: return "N/A";
    }
}

sf::Vector2f Button::toFloat(const sf::Vector2i iV) {
    return sf::Vector2f {float(iV.x), float(iV.y)};
}

const sf::Font Button::font          {"res/fonts/Roboto/Roboto-Regular.ttf"};
const float Button::radius           {28.f};
// const sf::Color Button::MAIN_BG      {254, 149,   5}; // Main Button Color
// const sf::Color Button::SEC_BG       {165, 165, 165}; // Secondary Button Color
// const sf::Color Button::NUM_BG       { 51,  50,  50}; // Num Button Color
// const sf::Color Button::TEXT_COLOR   {255, 255, 255}; // Text Color