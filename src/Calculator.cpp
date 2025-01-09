#include <Calculator.hpp>
#include <codecvt>

Calculator::Calculator(sf::RenderWindow* window):
    m_window(window)
    {

    }

void Calculator::init()
{
    // Initializing Buttons
    /////////////////////////////////////
    for (uint16_t i = 0; i < Calculator::nROWS; ++i)
    {
        for (uint16_t j = 0; j < Calculator::nCOLS; ++j)
        {
            Button& currButton = buttons[i][j];
            
            Button::Type currType = Button::Type((i * Calculator::nCOLS + j % Calculator::nCOLS));
            currButton.init(m_window, currType);
        }
    }
    // Initializing Text input
    /////////////////////////////////////
    m_input.setFillColor(sf::Color::White);
    m_input.setPosition({50, 50});
    m_input.setCharacterSize(50);
}

void Calculator::updateAndRender()
{
    // Render
    ///////////////////////////////////// 
    m_window->draw(m_input);
    for (uint16_t i = 0; i < Calculator::nROWS; ++i)
    {
        for (uint16_t j = 0; j < Calculator::nCOLS; ++j)
        {
            buttons[i][j].draw();
        }
    }
    ///////////////////////////////////// 
}


void Calculator::mouseMoved(const sf::Vector2i& pos) {
    for (uint16_t i = 0; i < nROWS; ++i)
    {
        for (uint16_t j = 0; j < nCOLS; ++j)
        {
            Button& currButton = buttons[i][j];
            if (currButton.contains(pos)) {
                if (!currButton.isHovered())
                    currButton.hover();
            }
            else if (currButton.isHovered())
                currButton.release();
        }
    }
}

void Calculator::mousePressed(const sf::Vector2i& pos) {
    for (uint16_t i = 0; i < nROWS; ++i)
    {
        for (uint16_t j = 0; j < nCOLS; ++j)
        {
            Button& currButton = buttons[i][j];
            if (currButton.contains(pos)) {
                if (!currButton.isPressed())
                    currButton.press();  
            }
            else if (currButton.isPressed())
                currButton.release();
        }
    }
}

void Calculator::buttonPressed(char32_t c) {
    // std::cout << "Entered Text: " << static_cast<char>(c) << "\n";
    Button::Type type = getType(c);
    if (type == Button::Type::NON)
        return;
    Button& button = buttons[type / nCOLS][type % nCOLS];
    if (!button.isPressed())
        button.press();
    // uint16_t i = type / Calculator::nCOLS;
    // uint16_t j = type % Calculator::nCOLS;

    if (isNum(type))
    {
        /* TODO: Check Num Limits*/
        if (currOperand.length() > 7)
            return;
        currOperand += c;
    }
    else if (type == Button::Type::DOT)
    {
        if (lastType == Button::Type::DOT)
            return;
        currOperand += c;
    }
    /*
        //  USER ENTERS DIGIT 
        if (cType == char_t::DIGIT || cType == char_t::DOT) {
            if (!operand.pushable() ||
                (cType == char_t::DOT && prev_cType == char_t::DOT))
                continue;
            operand += c;
        }
    */
    else if (isOperator(type))
    {
        if (isOperator(lastType))
            return;
        operands.append(atof(currOperand.data()));
        currOperand = "";
        operators.append(type);
    }

    /*
        // USER ENTERS OPERATOR 
        else if (isop(cType)) {
            if (isop(prev_cType))
                continue;
            operands.append(operand.toFloat());
            operand.clear();
            operators.append(cType);
        }
    */
    if (isOperator(type))
        m_input.setString(m_input.getString() + " " + c + " ");
    else if (isNum(type) || type == Button::Type::DOT)
        m_input.setString(m_input.getString() + c);
    /*
        //  PRINT CHARACTER IF VALID
        if (isop(cType))
            std::cout << " " << c << " ";
        else if (cType == char_t::DIGIT || cType == char_t::DOT)
            std::cout << c;
    */
   lastType = type;
}

void Calculator::mouseReleased(const sf::Vector2i& pos) {
    for (uint16_t i = 0; i < nROWS; ++i)
    {
        for (uint16_t j = 0; j < nCOLS; ++j)
        {
            Button& currButton = buttons[i][j];
            if (currButton.contains(pos) && currButton.isPressed()) {
                currButton.release();  
        }
    }
    }
}

// Static Data
///////////////////////////////////// 
const sf::Font Calculator::font {"res/fonts/Roboto/Roboto-Regular.ttf"};
// const uint16_t Calculator::nROWS {5};
// const uint16_t Calculator::nCOLS {4};
// const sf::Color Calculator::BG_COLOR {0, 0, 0}; // Calculator color
const float Calculator::hMargin {11.f};        // horizontal margin
const float Calculator::vMargin {11.f};        // vertical margin
const float Calculator::bottomMargin {50.f};
const sf::Vector2u Calculator::windowSize {800u, 600u};

sf::Vector2f Calculator::getPosOfButton(uint16_t i, uint16_t j) {
    sf::Vector2f vec {
        hMargin + j * (hMargin + Button::radius * 2) + Button::radius,
        bottomMargin + (nCOLS - i) * (vMargin + Button::radius * 2) + Button::radius
    };
    vec.y = windowSize.y - vec.y;
    std::cout << "pos[" << i << "][" << j << "]: " << vec.x << ", " << vec.y << "\n";
    return vec;
}

sf::Vector2f Calculator::round(const sf::Vector2f vector) {
    return sf::Vector2f{ std::round(vector.x), std::round(vector.y) };
}

bool Calculator::isNum(Button::Type type) {
    return  type == Button::Type::ZERO || type == Button::Type::ONE ||
            type == Button::Type::TWO || type == Button::Type::THREE ||
            type == Button::Type::FOUR || type == Button::Type::FIVE ||
            type == Button::Type::SIX || type == Button::Type::SEVEN ||
            type == Button::Type::EIGHT || type == Button::Type::NINE;
}

bool Calculator::isOperator(Button::Type type) {
    return  type == Button::Type::PLUS || type == Button::Type::MINUS ||
            type == Button::Type::CROSS || type == Button::Type::DIVIDE ||
            type == Button::Type::MOD;
}


bool Calculator::isSpace(const sf::Keyboard::Key& key) {
    return key == sf::Keyboard::Key::Space;
}

Button::Type Calculator::getType(char32_t c) {
    // is a number

    // Convert char32_t to UTF-8 (std::string, which uses char)
    // std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    // std::string utf8Str = converter.to_bytes(c32);

    // // If you're sure the char32_t is ASCII-compatible:
    // char c = utf8Str[0];
    // std::cout << "c= " << c << "\n";

    // if (c >= U'0' && c <= U'9') {
    //     std::cout << "num\n";
    //     return Button::Type(Button::Type::ZERO + c - U'0');
    // }
    
    switch (c) 
    {
        case U'0': return Button::Type::ZERO;
        case U'1': return Button::Type::ONE;
        case U'2': return Button::Type::TWO;
        case U'3': return Button::Type::THREE;
        case U'4': return Button::Type::FOUR;
        case U'5': return Button::Type::FIVE;
        case U'6': return Button::Type::SIX;
        case U'7': return Button::Type::SEVEN;
        case U'8': return Button::Type::EIGHT;
        case U'9': return Button::Type::NINE;
        case U'.': return Button::Type::DOT;
        case U'+': return Button::Type::PLUS;
        case U'-': return Button::Type::MINUS;
        case U'*': return Button::Type::CROSS;
        case U'/': return Button::Type::DIVIDE;
        case U'^': return Button::Type::POWER;
        case U'=': return Button::Type::EQUAL;
        case static_cast<char32_t>(13): case static_cast<char32_t>(10): return Button::Type::EQUAL;
        case  static_cast<char32_t>(8): case static_cast<char32_t>(27): return Button::Type::AC;
        default: return Button::Type::NON;
    }
}