#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>

class Bird {
    public:
        Bird(){
            this->x = rand() % 700 + 0;
            this->y = rand() % 500 + 0;
            this->dx = 3;
            this->dy = -3;
        
            this->triangle.setRadius(15);
            this->triangle.setPointCount(3);
            this->triangle.setOutlineColor(sf::Color::Blue);
            this->triangle.setOutlineThickness(2.f);
            this->triangle.setFillColor(sf::Color::Green);
        }

        sf::CircleShape getBird(){
            return this->triangle;
        }

        void checkBorders() {
            tempX = x + dx;
            tempY = y + dy;

            if (tempY <= 0) {
                dy *= -1;
            }

            if (tempX > 750) {
                dx *= -1;
            }

            if (tempY > 570) {
                dy *= -1;
            }

            if (tempX < 0) {
                dx *= -1;
            }
        }

        void moveBird() {
            checkBorders();
            x += dx;
            y += dy;
            triangle.setPosition(x, y);
        }

    private:
        float x, y, dx, dy, tempX, tempY;
        sf::CircleShape triangle;
    
};

int main()
{
    int height = 600;
    int width = 800;
    sf::RenderWindow window(sf::VideoMode(width, height), "Boids");

    // sf::CircleShape triangle(15, 3);
    // std::vector<sf::CircleShape> boids;

    // triangle.setOutlineColor(sf::Color::Blue);
    // triangle.setOutlineThickness(2.f);
    // triangle.setPosition(width/2, height/2);
    window.setFramerateLimit(60);
    
    // triangle.setFillColor(sf::Color::Green);
    sf::Clock clock;
    Bird boid;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        boid.moveBird();
    
        window.clear();
        window.draw(boid.getBird());
        window.display();
    }

    return 0;
}