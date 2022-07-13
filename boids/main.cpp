#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>

class Bird {
    public:
        Bird(){
            x = rand() % 700 + 0;
            y = rand() % 500 + 0;
            dx = rand() % 3 + (-3);
            dy = rand() % 3 + (-3);
            radius = 50;
            accelerationVec = sf::Vector2f(dx, dy);
        
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

        void renderVec(sf::Vector2f position) {
        }

        sf::Vector2f getPos() {
            return sf::Vector2f(x, y);
        }

        void setColor() {
            this->triangle.setFillColor(sf::Color::Red);    
        }

        sf::Vector2f getAccelerationVec() {
            return sf::Vector2f(dx, dy);
        }

        void setAccelerationVec(sf::Vector2f newAcceleration) {
            dx = newAcceleration.x;
            dy = newAcceleration.y;
        }


    private:
        float x, y, dx, dy, tempX, tempY;
        float radius;
        sf::Vector2f accelerationVec;
        sf::CircleShape triangle;
    
};

void calcDist();

int main()
{
    // sf::CircleShape rad(50, 300);
    // rad.setPosition(100, 100);
    // rad.setFillColor(sf::Color::White);
    srand(time(0));
    int height = 600;
    int width = 800;
    sf::RenderWindow window(sf::VideoMode(width, height), "Boids");
    window.setFramerateLimit(60);

    int countOfBoids = 50; // Set count of boids

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

        for (int i = 0; i < countOfBoids; i++) {
            std::vector<sf::Vector2f> vectors;
            int birdsInRadius = 0;
            Bird mainBird = flock[i];
            sf::Vector2f vecOfAcceleration = mainBird.getAccelerationVec();
            for (int j = 0; j < countOfBoids; j++) {
                if (mainBird.getPos() != flock[j].getPos()) {
                    float dist = sqrt(pow(mainBird.getPos().x - flock[j].getPos().x, 2) + 
                    pow(mainBird.getPos().y - flock[j].getPos().y, 2));
                    
                    if (dist < 50 && vectors.size() == 3) {
                        for (sf::Vector2f vec : vectors) {
                            vecOfAcceleration += vec;
                        }
                    } else if (dist < 50) {
                        vectors.push_back(flock[j].getPos());
                    }
                }
            }
            flock[i].setAccelerationVec(vecOfAcceleration);
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

void calcDist(Bird *flock, int countOfBird) {
    for (int i = 0; i < countOfBird; i++) {
        std::vector<Bird> flockmates;
        int birdsInRadius = 0;
        Bird mainBird = flock[i];
        for (int j = 0; j < countOfBird; j++) {
            if (mainBird.getPos() != flock[j].getPos()) {
                float dist = sqrt(pow(mainBird.getPos().x - flock[j].getPos().x, 2) - 
                pow(mainBird.getPos().y - flock[j].getPos().y, 2));

                if (dist < 50 && birdsInRadius == 3) {
                    mainBird.setColor();
                } else if (dist < 50) {
                    birdsInRadius++;
                }
            }
            
        }

    }
}