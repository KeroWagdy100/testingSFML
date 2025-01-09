#include <SFML/Graphics.hpp>
#include <EventHandling.hpp>
#include <Calculator.hpp>

// #include "EventHandling.cpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "Testing");
    Calculator calculator(&window);
    EventHandler evHandler(&window, &calculator);
    calculator.init();
    ///////////////////////////////////
    
    while (window.isOpen())
    {
        ///////////////////////////////////
        // Event Handling
        evHandler.handleEvents();
        ///////////////////////////////////
        // Clearing
        window.clear();
        ///////////////////////////////////
        // Drawing
        evHandler.drawLog();
        calculator.updateAndRender();
        ///////////////////////////////////
        // Displaying
        window.display();
    }
}
