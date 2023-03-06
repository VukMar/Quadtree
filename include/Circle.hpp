#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <SFML/Graphics.hpp>

class Circle
{

private:

float speed = 0;

float directionX = 1;
float directionY = 1;

float LeftX;
float RightX;

float TopY;
float BottomY;

bool BoundaryXset = false;
bool BoundaryYset = false;

sf::Color CollisionColor;
sf::Color MainColor;

int ID;

sf::CircleShape circle;

public:

bool inCollision = false;

Circle();

Circle(sf::Vector2f Position, float Radius, int ID);

sf::Vector2f getPosition();

float getRadius();

void setColor(sf::Color Color);

void setCollisionColor(sf::Color Color);

void drawTo(sf::RenderWindow &win);

int getID();

void setSpeed(float Speed);

void setBoundaryX(float left, float Right);

void setBoundaryY(float Top, float Bottom);

void move(float deltaTime);

~Circle();

};

#endif