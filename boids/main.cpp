#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <time.h>

class Bird {
    public:
        Bird(){
            x = rand() % 700 + 0;
            y = rand() % 500 + 0;
            dx = rand() % 3 + (-3);
            dy = rand() % 3 + (-3);
        
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

            if (tempY <= 0 || tempY > 570) {
                dy *= -1;
            }

            if (tempX > 750 || tempX < 0) {
                dx *= -1;
            }
        }

        void moveBird() {
            checkBorders();
            x += dx;
            y += dy;
            triangle.setPosition(sf::Vector2f(x, y));
        }
        void setX(float newX) {
            x = newX;
        }

    private:
        float x, y, dx, dy, tempX, tempY;
        sf::CircleShape triangle;
    
};

int main()
{
    srand(time(0));
    int height = 600;
    int width = 800;
    sf::RenderWindow window(sf::VideoMode(width, height), "Boids");
    window.setFramerateLimit(60);

    int countOfBoids = 20; // Set count of boids

    Bird *flock = new Bird[countOfBoids];
    for (int i = 0; i < countOfBoids; i++)
    {
        flock[i] = Bird();
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (int i = 0; i < countOfBoids; i++)
        {
            flock[i].moveBird();
        }
        
    
        window.clear();
        for (int i = 0; i < countOfBoids; i++)
        {
            window.draw(flock[i].getBird());
        }
        window.display();
    }
    delete[] flock;
    return 0;
}