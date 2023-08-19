#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>

sf::Color getContrastingColor(const sf::Color& color) {
    int threshold = 150; // Threshold for determining if a color is "dark" or "light"
    int sum = color.r + color.g + color.b;
    return sum > threshold * 3 ? sf::Color::Black : sf::Color::White;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Clock and Date", sf::Style::Resize);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }

    sf::Text dateText("", font, 20); // Text for displaying the date
    dateText.setFillColor(sf::Color::Black);
    dateText.setPosition(20, 20); // Position of the date text

    sf::RectangleShape colorButton(sf::Vector2f(100, 40)); // Button for changing background color
    colorButton.setFillColor(sf::Color(169, 169, 169)); // Grey color
    colorButton.setPosition(20, 80);

    sf::Text colorButtonText("Colors", font, 16); // Text for the button label
    colorButtonText.setFillColor(sf::Color::Black);
    sf::FloatRect textBounds = colorButtonText.getLocalBounds();
    colorButtonText.setPosition(colorButton.getPosition() + sf::Vector2f(colorButton.getSize().x / 2 - textBounds.width / 2, colorButton.getSize().y / 2 - textBounds.height / 2));

    sf::Color backgroundColor = sf::Color::White; // Initial background color

    std::vector<sf::Color> colorOptions = {
            sf::Color::White, sf::Color::Black, sf::Color::Red, sf::Color::Green,
            sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan,
            sf::Color(128, 128, 128), sf::Color(255, 165, 0) // Gray and Orange
    };

    bool showColorSelection = false; // Flag to show/hide color selection

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (colorButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    showColorSelection = !showColorSelection; // Toggle color selection window
                }

                // Check color selection window clicks
                if (showColorSelection && event.mouseButton.button == sf::Mouse::Left) {
                    for (size_t i = 0; i < colorOptions.size(); ++i) {
                        float yPos = 120 + static_cast<float>(i * 30);
                        if (event.mouseButton.x >= 20 && event.mouseButton.x <= 120 && event.mouseButton.y >= yPos && event.mouseButton.y <= yPos + 25) {
                            backgroundColor = colorOptions[i];
                            showColorSelection = false;
                        }
                    }
                }
            }

            if (event.type == sf::Event::Resized) {
                // Update the view to match the new window size
                sf::View view = window.getView();
                view.setSize(event.size.width, event.size.height);
                window.setView(view);
            }
        }

        // Calculate clock hand angles based on the current time
        float hourAngle, minuteAngle, secondAngle;

        // Get the current time
        std::time_t currentTime = std::time(nullptr);
        std::tm* timeinfo = std::localtime(&currentTime);

        hourAngle = (timeinfo->tm_hour % 12 + timeinfo->tm_min / 60.0) * (360.0 / 12.0) * (3.14159265 / 180.0) - (3.14159265 / 2.0);
        minuteAngle = (timeinfo->tm_min + timeinfo->tm_sec / 60.0) * (360.0 / 60.0) * (3.14159265 / 180.0) - (3.14159265 / 2.0);
        secondAngle = timeinfo->tm_sec * (360.0 / 60.0) * (3.14159265 / 180.0) - (3.14159265 / 2.0);

        // Calculate contrasting color for clock elements
        sf::Color handColor = backgroundColor == sf::Color::Green ? sf::Color::Black : getContrastingColor(backgroundColor);

        // Calculate contrasting color for date text
        sf::Color dateTextColor = backgroundColor == sf::Color::Green ? sf::Color::Black : getContrastingColor(backgroundColor);

        // Clear the window with the selected background color
        window.clear(backgroundColor);

        // Update clock element positions and sizes based on window size
        float clockRadius = std::min(window.getSize().x, window.getSize().y) * 0.3;
        sf::Vector2f clockCenter(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

        // Draw clock face
        sf::CircleShape clockFace(clockRadius);
        clockFace.setFillColor(backgroundColor);
        clockFace.setOutlineColor(handColor);
        clockFace.setOutlineThickness(2);
        clockFace.setOrigin(clockRadius, clockRadius);
        clockFace.setPosition(clockCenter);
        window.draw(clockFace);

        // Draw clock numbers
        for (int i = 1; i <= 12; ++i) {
            sf::Text numberText(std::to_string(i), font, 16);
            numberText.setFillColor(handColor);
            sf::FloatRect numberBounds = numberText.getLocalBounds();
            float angle = (i * 30.0 - 90.0) * (3.14159265 / 180.0);
            float x = clockCenter.x + (clockRadius - 40) * std::cos(angle) - numberBounds.width / 2;
            float y = clockCenter.y + (clockRadius - 40) * std::sin(angle) - numberBounds.height / 2;
            numberText.setPosition(x, y);
            window.draw(numberText);
        }

        // Draw clock hands
        sf::Vertex hourHand[] = {
                sf::Vertex(clockCenter),
                sf::Vertex(clockCenter + sf::Vector2f(60 * std::cos(hourAngle), 60 * std::sin(hourAngle)))
        };
        hourHand[0].color = handColor;
        hourHand[1].color = handColor;

        sf::Vertex minuteHand[] = {
                sf::Vertex(clockCenter),
                sf::Vertex(clockCenter + sf::Vector2f(80 * std::cos(        minuteAngle), 80 * std::sin(minuteAngle)))
        };
        minuteHand[0].color = handColor;
        minuteHand[1].color = handColor;

        sf::Vertex secondHand[] = {
                sf::Vertex(clockCenter),
                sf::Vertex(clockCenter + sf::Vector2f(90 * std::cos(secondAngle), 90 * std::sin(secondAngle)))
        };
        secondHand[0].color = handColor;
        secondHand[1].color = handColor;

        window.draw(hourHand, 2, sf::Lines);
        window.draw(minuteHand, 2, sf::Lines);
        window.draw(secondHand, 2, sf::Lines);

        // Update and display the date text
        std::ostringstream dateStream;
        dateStream << std::setw(2) << std::setfill('0') << timeinfo->tm_mday << "/" << std::setw(2) << std::setfill('0') << timeinfo->tm_mon + 1 << "/" << (timeinfo->tm_year + 1900); // Format the date as "day/month/year"
        dateText.setString(dateStream.str());
        dateText.setFillColor(dateTextColor); // Set the date text color
        window.draw(dateText);

        // Draw the color button and label
        window.draw(colorButton);
        window.draw(colorButtonText);

        // Display color selection window if toggled
        if (showColorSelection) {
            sf::RectangleShape selectionBackground(sf::Vector2f(120, 260));
            selectionBackground.setPosition(20, 110);
            selectionBackground.setFillColor(sf::Color(200, 200, 200, 220));
            window.draw(selectionBackground);

            for (size_t i = 0; i < colorOptions.size(); ++i) {
                float yPos = 120 + static_cast<float>(i * 30);
                sf::RectangleShape colorOption(sf::Vector2f(100, 25));
                colorOption.setPosition(20, yPos);
                colorOption.setFillColor(colorOptions[i]);
                window.draw(colorOption);
            }
        }

        // Display the content of the window
        window.display();
    }

    return 0;
}













