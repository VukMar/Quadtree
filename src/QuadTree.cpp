#include "../include/QuadTree.hpp"

namespace qt
{

///////////////////////////////////////////////////////////////////////////////////
//CLASS ELEMENT

Element::Element(){}

Element::Element(float x, float y, int ID)
{
    this->x = x;
    this->y = y;
    this->ID = ID;
}

Vec2f Element::getPosition()
{
    return Vec2f(this->x,this->y);
}

void Element::setPosition(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Element::setPosition(Vec2f Position)
{
    this->x = Position.x;
    this->y = Position.y;
}

int Element::getID()
{
    return this->ID;
}

void Element::setID(int ID)
{
    this->ID = ID;
}

void* Element::getObjectPointer()
{
    return this->Object;
}

void Element::setObjectPointer(void* Object)
{
    this->Object = Object;
}

Element::~Element(){}

///////////////////////////////////////////////////////////////////////////////////
//CLASS RECTANGLE

Rectangle::Rectangle(){}

Rectangle::Rectangle(float PosX, float PosY, float width, float height)
{
    this->StartingPoint = Vec2f(PosX,PosY);
    this->Size = Vec2f(width,height);
}

Vec2f Rectangle::getStartingPoint()
{
    return this->StartingPoint;
}

Vec2f Rectangle::getSize()
{
    return this->Size;
}

bool Rectangle::contains(Element Element)
{
    Vec2f Ppos = Element.getPosition();
    Vec2f Rpos = this->getStartingPoint();
    Vec2f Rsize = this->getSize();
    return Ppos.x >= Rpos.x && Ppos.x <= Rpos.x + Rsize.x &&
           Ppos.y >= Rpos.y && Ppos.y <= Rpos.y + Rsize.y;
}

Rectangle::~Rectangle(){}

///////////////////////////////////////////////////////////////////////////////////
//CLASS QUADTREENODE

QuadTreeNode::QuadTreeNode(Rectangle Boundary, int Capacity)
{
    this->Boundary = Boundary;
    this->Capacity = Capacity;
    for(int i = 0; i < 4; i++)
        this->Childern[i] = nullptr;
}


QuadTreeNode::QuadTreeNode(){}


QuadTreeNode::~QuadTreeNode(){}


void QuadTreeNode::subdivide()
{
    //Get boundary parameters
    Vec2f vBoundaryPos = Boundary.getStartingPoint();
    Vec2f vBoundarySize = Boundary.getSize();

    //Create NorthEast
    this->Childern[0] = new QuadTreeNode(Rectangle(vBoundaryPos.x,vBoundaryPos.y,vBoundarySize.x/2,vBoundarySize.y/2),this->Capacity);
    
    //Create NorthWest
    this->Childern[1] = new QuadTreeNode(Rectangle(vBoundaryPos.x + vBoundarySize.x/2,vBoundaryPos.y,vBoundarySize.x/2,vBoundarySize.y/2),this->Capacity);
    
    //Create SouthEast
    this->Childern[2] = new QuadTreeNode(Rectangle(vBoundaryPos.x,vBoundaryPos.y + vBoundarySize.y/2,vBoundarySize.x/2,vBoundarySize.y/2),this->Capacity);
    
    //Create SouthWest
    this->Childern[3] = new QuadTreeNode(Rectangle(vBoundaryPos.x + vBoundarySize.x/2,vBoundaryPos.y + vBoundarySize.y/2,vBoundarySize.x/2,vBoundarySize.y/2),this->Capacity);

    this->divided = true;
}

bool QuadTreeNode::insert(Element Element)
{
    if(!this->Boundary.contains(Element)){
        return false;
    }
    
    int size = this->Elements.size();
    
    if(size < this->Capacity){
        Elements.push_back(Element);
        return true;
    }
    else{
        if(!this->divided){
            subdivide();
        }
        for(int i = 0; i < 4; i++){
            if(this->Childern[i]->insert(Element)){
                return true;
            }
        } 
    }
    return false;
}

bool QuadTreeNode::isDivided(){
    return this->divided;
}

Rectangle QuadTreeNode::getBoundary(){
    return this->Boundary;
}

QuadTreeNode* QuadTreeNode::getChild(int Child){
    return this->Childern[Child];
}

std::vector<Element> QuadTreeNode::getElements(){
    return this->Elements;
}

void QuadTreeNode::clear(){
    
    this->Elements.clear();
    if(this->divided){
        for(int i = 0; i < 4; i++){
            this->Childern[i]->clear();
        }
    }
}

}