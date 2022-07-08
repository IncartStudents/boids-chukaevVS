#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Bird {
    public:
        Bird(){
            x = rand() % 700 + 0;
            y = rand() % 500 + 0;
            dx = 3;
            dy = -3;
        
            triangle.setRadius(15);
            triangle.setPointCount(3);
            triangle.setOutlineColor(sf::Color::Blue);
            triangle.setOutlineThickness(2.f);
            triangle.setFillColor(sf::Color::Green);
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
    window.setFramerateLimit(60);
    
    Bird boid;

    while (window.isOpen())
    {
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