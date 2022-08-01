#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>
#include <math.h>

class Bird {
    public:
        Bird(){
            x = rand() % 700 + 0;
            y = rand() % 500 + 0;
            dx = rand() % 3 + (-3);
            dy = rand() % 3 + (-3);
            accelerationVec = sf::Vector2f(dx - x, dy - y);
        
            triangle.setRadius(15);
            triangle.setPointCount(300);
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

            if (tempY <= 0 || tempY > 970) {
                dy *= -1;
            }

            if (tempX > 1550 || tempX < 0) {
                dx *= -1;
            }
        }

        void moveBird() {
            checkBorders();
            x += dx;
            y += dy;
            triangle.setPosition(sf::Vector2f(x, y));
        }


        sf::Vector2f getPos() {
            return sf::Vector2f(x + 15, y + 15);
        }

        sf::Vector2f getAccelerationVec() {
            return sf::Vector2f(dx - x, dy - y);
        }

        sf::Vector2f getAccelerationPoint() {
            return sf::Vector2f(dx, dy);
        }

        void setAccelerationVecSep(sf::Vector2f newAcceleration) {
            float speed = sqrt(pow(dx - x, 2) + pow(dy - y, 2));

            newAcceleration = newAcceleration / speed; // Limiting speed

            dx = (dx + newAcceleration.x * -0.05);
            dy = (dy + newAcceleration.y * -0.05);

        }

        void setAccelerationVecCoh(sf::Vector2f newAcceleration) {
            float speed = sqrt(pow(dx - x, 2) + pow(dy - y, 2));

            newAcceleration = newAcceleration / speed; // Limiting speed

            dx = (dx + newAcceleration.x * 0.01);
            dy = (dy + newAcceleration.y * 0.01);

        }

        void setAccelerationAlign(sf::Vector2f newAcceleration)
        {
            float speed = sqrt(pow(dx - x, 2) + pow(dy - y, 2));

            //newAcceleration = newAcceleration / speed; // Limiting speed

            // dx = (dx + newAcceleration.x * 0.01);
            // dy = (dy + newAcceleration.y * 0.01);

            dx += (newAcceleration.x - dx) * -0.05;
            dy += (newAcceleration.y - dy) * -0.05;
        }

        void limitSpeed()
        {
            int speedLimit = 6;
            float speed = sqrt(pow(dx, 2) + pow(dy, 2));

            if(speed > speedLimit)
            {
                dx = dx / speed * speedLimit;
                dy = dy / speed * speedLimit;
            }
        }

        void normalizeAccelertion() {
            while((dx > 6 || dx < -6) && (dy > 6 || dy < -6)) {
                if (dx > 6)
                {
                    dx -= 1;
                }
                if (dx < -6)
                {
                    dx += 1;
                }
                if (dy > 6)
                {
                    dy -= 1;
                }
                if (dy < -6)
                {
                    dy += 1;
                }
            }
        }

        sf::Vertex getDirection() {
            sf::Vertex direction[] = {sf::Vertex(sf::Vector2f(x, y)), sf::Vertex(sf::Vector2f(dx, dy))};

            return *direction;
        }

        float getDx() {
            return dx;
        }

        float getDy() {
            return dy;
        }

        void setDx(float newDx) {
            dx += (newDx - dx) * 0.5;
        }

        void setDy(float newDy) {
            dy += (newDy - dy) * 0.5;
        }

    private:
        float x, y, dx, dy, tempX, tempY;
        float radius;
        float normDx, normDy;
        sf::Vector2f accelerationVec;
        sf::CircleShape triangle;
    
};

void ruleOfSeparation(int countOfBoids, Bird flock[])
{
    for (int i = 0; i < countOfBoids; i++) {
        std::vector<sf::Vector2f> vectors;
        int birdsInRadius = 0;
        Bird mainBird = flock[i];
        sf::Vector2f vecOfAcceleration = mainBird.getAccelerationVec();
        for (int j = 0; j < countOfBoids; j++) {
            if (mainBird.getPos() != flock[j].getPos()) {
                float dist = sqrt(pow(mainBird.getPos().x - flock[j].getPos().x, 2) + 
                pow(mainBird.getPos().y - flock[j].getPos().y, 2));
                if (dist < 200) {
                    vectors.push_back(sf::Vector2f(flock[j].getPos().x - mainBird.getPos().x, 
                    flock[j].getPos().y - mainBird.getPos().y));
                    
                    for (sf::Vector2f vec : vectors) {
                        vecOfAcceleration += vec;
                    }
                } 
                else {
                    flock[i].normalizeAccelertion();
                }
            }
        }
        flock[i].setAccelerationVecSep(vecOfAcceleration);
    }
}

void ruleOfCohesion(int countOfBoids, Bird flock[])
{
    for (int i = 0; i < countOfBoids; i++) {
        std::vector<sf::Vector2f> vectors;
        int birdsInRadius = 0;
        Bird mainBird = flock[i];
        sf::Vector2f vecOfAcceleration = mainBird.getAccelerationVec();
        for (int j = 0; j < countOfBoids; j++) {
            if (mainBird.getPos() != flock[j].getPos()) {
                float dist = sqrt(pow(mainBird.getPos().x - flock[j].getPos().x, 2) + 
                pow(mainBird.getPos().y - flock[j].getPos().y, 2));
                if (dist < 200) {
                    vectors.push_back(sf::Vector2f(flock[j].getPos().x - mainBird.getPos().x, 
                    flock[j].getPos().y - mainBird.getPos().y));
                    
                    for (sf::Vector2f vec : vectors) {
                        vecOfAcceleration += vec;
                    }
                } 
                else {
                    flock[i].normalizeAccelertion();
                }
            }
        }
        flock[i].setAccelerationVecCoh(vecOfAcceleration);
    }
}

void ruleOfAlignment(int countOfBoids, Bird flock[])
{
     for (int i = 0; i < countOfBoids; i++) {
        float avgDx = 0;
        float avgDy = 0;
        sf::Vector2f avgVelocity = flock[i].getAccelerationVec();
        int numOfNeighbors = 0;
        float angle = -0.01; // Some angle
        std::vector<Bird> neighbros;

        std::vector<sf::Vector2f> vectors;
        int birdsInRadius = 0;
        Bird mainBird = flock[i];
        sf::Vector2f vecOfAcceleration = mainBird.getAccelerationVec();
        for (int j = 0; j < countOfBoids; j++) {
            if (mainBird.getPos() != flock[j].getPos()) {
                
                float dist = sqrt(pow(mainBird.getPos().x - flock[j].getPos().x, 2) + 
                pow(mainBird.getPos().y - flock[j].getPos().y, 2));

                if (dist < 200) {
                    neighbros.push_back(flock[j]);
                    numOfNeighbors++;
                    avgVelocity += flock[j].getAccelerationVec();



                    float cosAngle = (flock[i].getAccelerationVec().x * flock[j].getAccelerationVec().x + 
                        flock[i].getAccelerationVec().y * flock[j].getAccelerationVec().y) / 
                        (sqrt(pow(flock[i].getAccelerationVec().x, 2) + pow(flock[i].getAccelerationVec().y, 2)) * 
                        sqrt(pow(flock[j].getAccelerationVec().x, 2) + pow(flock[j].getAccelerationVec().y, 2)));

                    
                    // if (cosAngle < 1 && cosAngle > 0.996195)
                    // {
                    //     // flock[j].setDx(cos(angle) * flock[j].getDx() - sin(angle) * flock[j].getDy());
                    //     // flock[j].setDy(sin(angle) * flock[j].getDx() + cos(angle) * flock[j].getDy());

                    //     // flock[j].setDx(cos(angle) * flock[j].getAccelerationVec().x -
                    //     //     sin(angle) * flock[j].getAccelerationVec().y);
                    //     // flock[j].setDy(sin(angle) * flock[j].getAccelerationVec().x +
                    //     //     cos(angle) * flock[j].getAccelerationVec().y);

                    //     flock[j].setAccelerationAlign(sf::Vector2f(cos(angle) * flock[j].getAccelerationVec().x -
                    //         sin(angle) * flock[j].getAccelerationVec().y, 
                    //         sin(angle) * flock[j].getAccelerationVec().x +
                    //         cos(angle) * flock[j].getAccelerationVec().y));
                    //     flock[j].limitSpeed();
                    // }
                } 
            }
        }
        if (neighbros.size() > 0)
        {
            bool flag = false;
            for (Bird bird : neighbros)
            {
                Bird copyMain = flock[i];
                Bird copyNeighbor = bird;

                copyMain.limitSpeed();
                bird.limitSpeed();
                if (copyMain.getAccelerationVec() == copyNeighbor.getAccelerationVec())
                {
                    flag = true;
                }
            }

            if(flag)
            {
                flock[i].limitSpeed();
            } else {
                avgVelocity.x = avgVelocity.x / numOfNeighbors;
                avgVelocity.y = avgVelocity.y / numOfNeighbors;

                flock[i].setAccelerationAlign(avgVelocity);
            }
            
        } else flock[i].limitSpeed();
    }
}

int main()
{
    srand(time(0));
    int height = 1000;
    int width = 1600;
    sf::RenderWindow window(sf::VideoMode(width, height), "Boids");
    window.setFramerateLimit(60);

    int countOfBoids = 100; // Set count of boids

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

        // ruleOfSeparation(countOfBoids, flock);
        // ruleOfCohesion(countOfBoids, flock);
        ruleOfAlignment(countOfBoids, flock);

        for (int i = 0; i < countOfBoids; i++)
        {
            flock[i].moveBird();
        }
        
    
        window.clear();
        for (int i = 0; i < countOfBoids; i++)
        {
            window.draw(flock[i].getBird());

            sf::VertexArray dir(sf::Lines, 2); // Func that draw the direction of each boid
            dir[0].position = sf::Vector2f(flock[i].getPos().x + flock[i].getAccelerationPoint().x * 10,
                flock[i].getPos().y + flock[i].getAccelerationPoint().y * 10);
            dir[1].position = sf::Vector2f(flock[i].getPos().x, flock[i].getPos().y);

            window.draw(dir);
        }
        window.display();
    }
    delete[] flock;
    return 0;
}
