#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Clock and Date");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }

    sf::Text dateText("", font, 20); // Text for displaying the date
    dateText.setFillColor(sf::Color::Black);
    dateText.setPosition(20, 20); // Position of the date text

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window
        window.clear(sf::Color::White);

        // Update clock elements based on window size
        sf::Vector2f center(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
        float clockRadius = std::min(window.getSize().x, window.getSize().y) / 3.0f;

        // Draw clock circle
        sf::CircleShape clockCircle(clockRadius);
        clockCircle.setFillColor(sf::Color::Transparent);
        clockCircle.setOutlineColor(sf::Color::Black);
        clockCircle.setOutlineThickness(2);
        clockCircle.setOrigin(clockRadius, clockRadius);
        clockCircle.setPosition(center);
        window.draw(clockCircle);

        // Draw clock numbers
        for (int i = 1; i <= 12; ++i) {
            sf::Text number(std::to_string(i), font, clockRadius / 10);
            number.setFillColor(sf::Color::Black);
            number.setOrigin(number.getGlobalBounds().width / 2, number.getGlobalBounds().height / 2);

            float angle = i * (360.0f / 12.0f) - 90.0f;
            float x = center.x + (clockRadius - clockRadius / 10) * cos(angle * (3.14159265359f / 180.0f));
            float y = center.y + (clockRadius - clockRadius / 10) * sin(angle * (3.14159265359f / 180.0f));

            number.setPosition(x, y);
            window.draw(number);
        }

        // Get the current time
        std::time_t currentTime = std::time(nullptr);
        std::tm* timeinfo = std::localtime(&currentTime);

        // Calculate clock hand angles based on the current time
        float seconds = timeinfo->tm_sec;
        float minutes = timeinfo->tm_min + seconds / 60.0f;
        float hours = timeinfo->tm_hour % 12 + minutes / 60.0f;

        // Draw clock hands
        sf::Vertex hourHand[] = {
                sf::Vertex(center, sf::Color::Black),
                sf::Vertex(center + sf::Vector2f((clockRadius * 0.5) * cos((hours * 30 - 90) * (3.14159265359f / 180.0f)), (clockRadius * 0.5) * sin((hours * 30 - 90) * (3.14159265359f / 180.0f))), sf::Color::Black)
        };

        sf::Vertex minuteHand[] = {
                sf::Vertex(center, sf::Color::Black),
                sf::Vertex(center + sf::Vector2f((clockRadius * 0.7) * cos((minutes * 6 - 90) * (3.14159265359f / 180.0f)), (clockRadius * 0.7) * sin((minutes * 6 - 90) * (3.14159265359f / 180.0f))), sf::Color::Black)
        };

        sf::Vertex secondHand[] = {
                sf::Vertex(center, sf::Color::Red),
                sf::Vertex(center + sf::Vector2f((clockRadius * 0.9) * cos((seconds * 6 - 90) * (3.14159265359f / 180.0f)), (clockRadius * 0.9) * sin((seconds * 6 - 90) * (3.14159265359f / 180.0f))), sf::Color::Red)
        };

        window.draw(hourHand, 2, sf::Lines);
        window.draw(minuteHand, 2, sf::Lines);
        window.draw(secondHand, 2, sf::Lines);

        // Update and display the date text
        std::ostringstream dateStream;
        dateStream << std::setw(2) << std::setfill('0') << timeinfo->tm_mday << "/" << std::setw(2) << std::setfill('0') << timeinfo->tm_mon + 1 << "/" << (timeinfo->tm_year + 1900); // Format the date as "day/month/year"
        dateText.setString(dateStream.str());
        window.draw(dateText);

        // Display the content of the window
        window.display();
    }

    return 0;
}














