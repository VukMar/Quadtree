#include "../include/Circle.hpp"

//Class Circle

Circle::Circle(){}

Circle::Circle(sf::Vector2f Position, float Radius, int ID)
{
    this->circle.setPosition(Position.x,Position.y);
    this->circle.setRadius(Radius);
    this->circle.setOrigin(Radius,Radius);
    this->ID = ID;
}

sf::Vector2f Circle::getPosition()
{
    return this->circle.getPosition();
}

float Circle::getRadius()
{
    return this->circle.getRadius();
}

void Circle::setColor(sf::Color Color)
{
    this->MainColor = Color;
}

void Circle::setCollisionColor(sf::Color Color)
{
    this->CollisionColor = Color;
}

void Circle::drawTo(sf::RenderWindow &win)
{
    (inCollision)? this->circle.setFillColor(this->CollisionColor) : this->circle.setFillColor(this->MainColor);
    win.draw(this->circle);
}

int Circle::getID()
{
    return this->ID;
}

void Circle::setSpeed(float Speed)
{
    this->speed = Speed;
}

void Circle::setBoundaryX(float Left, float Right)
{
    if(Left > this->circle.getPosition().x)
    {
        Left = this->circle.getPosition().x;
    }
    if(Right < this->circle.getPosition().x)
    {
        Right = this->circle.getPosition().x;
    }
    this->LeftX = Left;
    this->RightX = Right;
    BoundaryXset = true;
}

void Circle::setBoundaryY(float Top, float Bottom)
{
    if(Top > this->circle.getPosition().y)
    {
        Top = this->circle.getPosition().y;
    }
    if(Bottom < this->circle.getPosition().y)
    {
        Bottom = this->circle.getPosition().y;
    }
    this->TopY = Top;
    this->BottomY = Bottom;
    BoundaryYset = true;
}

void Circle::move(float deltaTime)
{
    // Move the circle by a specified amount
    float movementX = this->speed * deltaTime * this->directionX;
    float movementY = this->speed * deltaTime * this->directionY;
    if(!BoundaryXset)
        movementX = 0;
    if(!BoundaryYset)
        movementY = 0;
    
    circle.move(movementX, movementY);

    //Change direction
    if(this->circle.getPosition().x < this->LeftX || this->circle.getPosition().x > this->RightX) 
    {
        this->directionX *= -1;
    }
    if(this->circle.getPosition().y < this->TopY || this->circle.getPosition().y > this->BottomY) 
    {
        this->directionY *= -1;
    }
}

Circle::~Circle(){}