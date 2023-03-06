#ifndef SFML_APP_HPP
#define SFML_APP_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "../include/QuadTree.hpp"
#include "../include/CyberFont.hpp"
#include "../include/Circle.hpp"
class App
{
private:

sf::RenderWindow win;

std::string CheckStr;
int CheckMade = 0;
int Collisions = 0;
bool DrawRects = true;
bool ByVector = true;

sf::Clock DeltaTimeClock;
float deltaTime;

//Text display
CyberFont CyberFont;
std::vector<unsigned char> FontData;
sf::Font font;
sf::Text ComputationTimeInfo;

std::chrono::time_point<std::chrono::system_clock> timePoint;
std::chrono::duration<float> Dduration;

std::vector<Circle> Circles;

//Vector to store collided circles
std::vector<std::pair<Circle*,Circle*>> OverlapPairs;

void HandleEvents(sf::Event &ev);

bool CirclesOverlap(Circle circle, Circle target);

void checkByVector();

void drawByVector();

void checkQuadTreeCircles(qt::QuadTreeNode &Qtree, qt::Element &Element);

void checkByQuadTree(qt::QuadTreeNode &Qtree);

void drawByQTree(qt::QuadTreeNode &Qtree);

void windowLogic(std::stringstream &InfoStream, qt::QuadTreeNode &Qtree);

void windowDrawing(std::stringstream &InfoStream, qt::QuadTreeNode &Qtree);

public:

App();

void run();

~App();

};


#endif