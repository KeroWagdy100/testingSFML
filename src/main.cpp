#include <SFML/Graphics.hpp>
#include "EventHandling.cpp"

int main()
{
    // auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "CMake SFML Project");
    // window.setFramerateLimit(60);

    // while (window.isOpen())
    // {
    //     while (const std::optional event = window.pollEvent())
    //     {
    //         if (event->is<sf::Event::Closed>())
    //         {
    //             window.close();
    //         }
    //     }

    //     window.clear();
    //     window.display();
    // }
    App app;
    app.run();
}
