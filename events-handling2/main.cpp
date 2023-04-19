#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>

class CustomRectangleShape : public sf::RectangleShape
{
public:
    CustomRectangleShape(sf::Vector2f size, sf::Vector2f position)
    {
        setSize(size);
        setPosition(position);
        setOrigin(size / 2.0f);
        setFillColor(sf::Color(0, 255, 0));
    }


    void moveInDirection(const sf::Time& elapsed, const sf::Keyboard::Key& key)
    {
        float dx = 0.0f, dy = 0.0f;

        switch (key)
        {
        case sf::Keyboard::Left:
            dx = -m_speed;
            break;
        case sf::Keyboard::Right:
            dx = m_speed;
            break;
        case sf::Keyboard::Up:
            dy = -m_speed;
            break;
        case sf::Keyboard::Down:
            dy = m_speed;
            break;
        default:
            std::cerr << "Invalid key pressed" << std::endl;
            return;
        }

        sf::Vector2f pos = getPosition();
        pos.x += dx * elapsed.asSeconds();
        pos.y += dy * elapsed.asSeconds();
        setPosition(pos);
        setBounds(m_minX, m_maxX, m_minY, m_maxY);
    }


    bool isClicked(sf::Vector2i& mouse_position) const
    {
        return getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_position));
    }

    void setBounds(float minX, float maxX, float minY, float maxY)
    {
        m_minX = minX;
        m_maxX = maxX;
        m_minY = minY;
        m_maxY = maxY;

        sf::Vector2f pos = getPosition();
        pos.x = std::min(std::max(pos.x, m_minX + getSize().x / 2.0f), m_maxX - getSize().x / 2.0f);
        pos.y = std::min(std::max(pos.y, m_minY + getSize().y / 2.0f), m_maxY - getSize().y / 2.0f);
        setPosition(pos);
    }

    void setSpeed(float speed, float minX, float maxX, float minY, float maxY)
    {
        m_speed = speed;
        m_minX = minX;
        m_maxX = maxX;
        m_minY = minY;
        m_maxY = maxY;
    }

    void setSelected(bool selected)
    {
        m_selected = selected;
        if (m_selected)
        {
            setFillColor(sf::Color::Red);
        }
        else
        {
            setFillColor(sf::Color(0, 255, 0));
        }
    }

    bool isSelected() const
    {
        return m_selected;
    }

private:
    float m_speed = 0.0f;
    float m_minX = 0.0f;
    float m_maxX = 0.0f;
    float m_minY = 0.0f;
    float m_maxY = 0.0f;
    bool m_selected = false;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML");

    std::srand(std::time(nullptr));

    std::vector<CustomRectangleShape> rectangles;

    for (int i = 0; i < 10; i++)
    {
        sf::Vector2f size(120.0, 60.0);
        sf::Vector2f position(std::rand() % (window.getSize().x - 120), std::rand() % (window.getSize().y - 60));
        rectangles.emplace_back(CustomRectangleShape(size, position));
    }
sf::Clock clock;

float minX = 0.0f;
float maxX = window.getSize().x;
float minY = 0.0f;
float maxY = window.getSize().y;

for (auto& rect : rectangles)
{
    float speed = static_cast<float>(std::rand() % 300 + 100);
    rect.setSpeed(speed, minX, maxX, minY, maxY);
}

CustomRectangleShape* selectedRectangle = nullptr;

while (window.isOpen())
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                for (auto& rect : rectangles)
                {
                    if (rect.isClicked(mousePosition))
                    {
                        if (selectedRectangle != nullptr)
                        {
                            selectedRectangle->setSelected(false);
                        }

                        rect.setSelected(true);
                        selectedRectangle = &rect;
                        break;
                    }
                }
            }
        }
    }

    sf::Time elapsed = clock.restart();

    if (selectedRectangle != nullptr)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            selectedRectangle->moveInDirection(elapsed, sf::Keyboard::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            selectedRectangle->moveInDirection(elapsed, sf::Keyboard::Right);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            selectedRectangle->moveInDirection(elapsed, sf::Keyboard::Up);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            selectedRectangle->moveInDirection(elapsed, sf::Keyboard::Down);
        }
    }

    window.clear();

    for (auto& rect : rectangles)
    {
        window.draw(rect);
    }

    window.display();
}
return 0;
}
