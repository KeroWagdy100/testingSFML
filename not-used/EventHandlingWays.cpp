#include <SFML/Graphics.hpp>

namespace {
    template <class T>
    std::string vec2ToString(const sf::Vector2<T>& v) {
        return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
    }
};

class App {
public:
    /////////////////////////////////////////////
    App() {
        m_window.setVerticalSyncEnabled(true);
        m_logText.setFillColor(sf::Color::White);

        m_handlerText.setFillColor(sf::Color::White);
        m_handlerText.setStyle(sf::Text::Bold);
        m_handlerText.setPosition({380.f, 260.f});
        
        m_instructions.setFillColor(sf::Color::White);
        m_instructions.setStyle(sf::Text::Bold);
        m_instructions.setPosition({380.f, 310.f});
    }

    /////////////////////////////////////////////
    void run() {
        // Main Loop
        while (m_window.isOpen()) {
            // handle events based on current handler type
            switch(m_handlerType) {
                case HandlerType::Classic: handleClassical(); break;
                case HandlerType::Visitor : handleVisitor(); break;
                case HandlerType::Overload : handleOverload(); break;
                case HandlerType::Forward : handleForward(); break;
                default: handleClassical();
            }
            /////////////////////////////////////////////
            m_window.clear();

            if (m_log.size() > 24u)
                m_log.erase(m_log.begin(), m_log.begin() + m_log.size() - 24);
            // Draw the contents of the log to the window
            for (size_t i = 0; i < m_log.size(); i++) {
                m_logText.setPosition({50.f, i * 20.f + 50.f});
                m_logText.setString(m_log.at(i));
                m_window.draw(m_logText);
            }
            m_window.draw(m_handlerText);
            m_window.draw(m_instructions);
            m_window.display();
        }
    }

    /////////////////////////////////////////////
    struct Visitor {
        /////////////////////////////////////////////
        // Structure handles events
        // an instance is passed to event->visit() in the event handling loop
        App& app;
        explicit Visitor(App& application): app(application) {}

        std::optional<std::string> operator()(const sf::Event::Closed&) {
            app.m_window.close();
            return std::nullopt;
        }
        
        std::optional<std::string> operator()(const sf::Event::MouseMoved& ev) {
            return "Mouse Moved: " + vec2ToString(ev.position);
        }

        std::optional<std::string> operator()(const sf::Event::KeyPressed& ev) {
            if (ev.code == sf::Keyboard::Key::Enter) {
                app.m_handlerType = HandlerType::Overload;
                app.m_handlerText.setString("Current Handler: Overload");
            }
            return "Key Pressed: " + sf::Keyboard::getDescription(ev.scancode);
        }

        std::optional<std::string> operator()(const sf::Event::MouseButtonPressed& ev) {
            return "Mouse Pressed\n";
        }

        std::optional<std::string> operator()(const sf::Event::TouchBegan& ev) {
            return "Touch Began: " + vec2ToString(ev.position);
        }

        /////////////////////////////////////////////
        // When defining a visitor, make sure all event types can be handled by it.
        // If you don't intend on exhaustively specifying an operator() for each
        // event type, you can provide a templated operator() that will be selected
        // by overload resolution when no other event type matches.
        template <class T>
        std::optional<std::string> operator()(const T&) {
            return std::nullopt;
        }
    };
private:
    enum class HandlerType {
        Classic,
        Visitor,
        Overload,
        Forward
    };

    /////////////////////////////////////////////
    // Handle Forward Implementations
    void handle(const sf::Event::Closed&) {
        m_window.close();
    }
    void handle(const sf::Event::KeyPressed& ev) {
        if (ev.code == sf::Keyboard::Key::Enter) {
            m_handlerType = HandlerType::Classic;
            m_handlerText.setString("Current Handler: Classic");
        }
        m_log.emplace_back("Key Pressed: " + sf::Keyboard::getDescription(ev.scancode));
    }
    void handle(const sf::Event::MouseMoved& ev) {
        m_log.emplace_back("Mouse Moved: " + vec2ToString(ev.position));
    }
    void handle(const sf::Event::MouseButtonPressed& ev) {
        m_log.emplace_back("Mouse Pressed");
    }
    void handle(const sf::Event::TouchBegan& ev) {
        m_log.emplace_back("Touch Began" + vec2ToString(ev.position));
    }
    template <class T>
    void handle(const T&) {}
    /////////////////////////////////////////////

    void handleClassical() {
        // The "classical" form of event handling
        // Poll/Wait for the events in a loop and handle them
        // individually based on their concrete type
        while (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
                break;
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                m_log.emplace_back("Key Pressed: " + sf::Keyboard::getDescription(keyPressed->scancode));
                if (keyPressed->code == sf::Keyboard::Key::Enter) {
                    m_handlerType = HandlerType::Visitor;
                    m_handlerText.setString("Current Handler: Visitor");
                }
            }

            else if (const auto* mousePos = event->getIf<sf::Event::MouseMoved>())
                m_log.emplace_back("Mouse Moved: " + vec2ToString(mousePos->position));

            else if (event->is<sf::Event::MouseButtonPressed>())
                m_log.emplace_back("Mouse Pressed");

            else if (const auto* touchBegan = event->getIf<sf::Event::TouchBegan>())
                m_log.emplace_back("Touch Began: " + vec2ToString(touchBegan->position));
        }
    }
    void handleVisitor() {
        // Event Visitor
        // A visitor able to visit all event types is passed to the event
        // The visitor's defined operator()s can also return values
        while (const std::optional event = m_window.pollEvent()) {
            if (std::optional logMessage = event->visit(Visitor(*this)))
                m_log.emplace_back(std::move(*logMessage));
        }
    }
    void handleOverload() {
        m_window.handleEvents(
            [&](const sf::Event::Closed&) {
            m_window.close();
            },

            [&](const sf::Event::KeyPressed& ev) {
                if (ev.code == sf::Keyboard::Key::Enter) {
                    m_handlerText.setString("Current Handler: Forward");
                }
                m_log.emplace_back("Key Pressed: " + sf::Keyboard::getDescription(ev.scancode));
            },

            [&](const sf::Event::MouseMoved& ev) {
                m_log.emplace_back("Mouse Moved: " + vec2ToString(ev.position));
            },
            
            [&](const sf::Event::MouseButtonPressed&) {
                m_log.emplace_back("Mouse Pressed");
            },
            
            [&](const sf::Event::TouchBegan& ev) {
                m_log.emplace_back("Touch Began: " + vec2ToString(ev.position));
            }
        );
    }
    void handleForward() {
        // Forward to other callable
        // In this case we forward it to our handle member functions
        // we defined for the concrete event types we want to handle
        // When choosing this method a default "catch-all" handler must
        // be available for unhandled events to be forwarded to
        // If you don't want to provide an empty "catch-all" handler
        // you will have to make sure (e.g. via if constexpr) that this
        // lambda doesn't attempt to call a member function that doesn't exist
        m_window.handleEvents([this](const auto& event){
            handle(event);
        });
    }

    /////////////////////////////////////////////
    // Member Data
    /////////////////////////////////////////////
    sf::RenderWindow    m_window{sf::VideoMode({800u, 600u}), "Handling Events"};
    const sf::Font      m_font{"res/TCB_____.TTF"};
    sf::Text            m_logText{m_font, "", 24};
    sf::Text            m_handlerText{m_font, "Current Handler: Classic", 24};
    sf::Text            m_instructions{m_font, "Press Enter to change handler type", 24};
    std::vector<std::string> m_log;
    HandlerType         m_handlerType{HandlerType::Classic};
};