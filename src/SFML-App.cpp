#include "../include/SFML-App.hpp"

//APLICATION

//Private:

void App::HandleEvents(sf::Event &ev)
{
    while(win.pollEvent(ev))
    {
        if(ev.type == sf::Event::Closed)
        {
            win.close();
        }
        if(ev.type == sf::Event::KeyReleased)
        {
            if(ev.key.code == sf::Keyboard::Space)
            {
                ByVector = !ByVector;
            }
            if(ev.key.code == sf::Keyboard::LControl)
            {
                DrawRects = !DrawRects;
            }
        }
    }
}

bool App::CirclesOverlap(Circle circle, Circle target)
{
    float r1 = circle.getRadius();
    float r2 = target.getRadius();

    sf::Vector2f cp = circle.getPosition();
    sf::Vector2f tp = target.getPosition();

    if(fabs((cp.x - tp.x)*(cp.x - tp.x)+(cp.y - tp.y)*(cp.y-tp.y)) < (r1+r2)*(r1+r2))
        return true;
    
    return false;
}

void App::checkByVector()
{
    CheckStr = "Check by Vector took: ";
    for(auto &CircleO : Circles)
    {
        for(auto &Target : Circles)
        {
            CheckMade++;
            if(CircleO.getID() != Target.getID())
            {
                if(CirclesOverlap(CircleO,Target))
                {
                    std::pair<Circle*,Circle*> pair;
                    pair.first = &CircleO;
                    pair.second = &Target;
                    OverlapPairs.push_back(pair);
                    break;
                }
            }
        }
    }
}

void App::drawByVector()
{
    CheckStr = "Draw by Vector took: ";
    for(auto &Circle : Circles)
    {   
        if(Circle.inCollision)
        {
            Collisions++;
        }
        Circle.drawTo(win);
    }
}

void App::checkQuadTreeCircles(qt::QuadTreeNode &Qtree, qt::Element &Element)
{
    if(Qtree.getBoundary().contains(Element))
    {   
        Circle* Obj = static_cast<Circle*>(Element.getObjectPointer());
        for(auto &Target : Qtree.getElements())
        {
            CheckMade++;
            Circle* TargetObj = static_cast<Circle*>(Target.getObjectPointer());
            if((*Obj).getID() != (*TargetObj).getID())
            {
                if(CirclesOverlap((*Obj), (*TargetObj)))
                {
                    std::pair<Circle*,Circle*> pair;
                    pair.first = Obj;
                    pair.second = TargetObj;
                    OverlapPairs.push_back(pair);
                    break;
                }
            }
        }
        if(Qtree.isDivided())
        {
            for(int i = 0; i < 4; i++)
            {
                qt::QuadTreeNode *node = Qtree.getChild(i);
                checkQuadTreeCircles(*node, Element);
            }
        }
    }
}

void App::checkByQuadTree(qt::QuadTreeNode &Qtree)
{
    CheckStr = "Check by QuadTree took: ";
    for(auto &Element : Qtree.getElements())
    {
        checkQuadTreeCircles(Qtree, Element);
    }
    if(Qtree.isDivided())
    {
        for(int i = 0; i < 4; i++)
        {
            qt::QuadTreeNode *node = Qtree.getChild(i);
            checkByQuadTree(*node);
        }
    }

}

void App::drawByQTree(qt::QuadTreeNode &Qtree)
{
    CheckStr = "Draw by QuadTree took: ";
    if(DrawRects)
    {
        sf::RectangleShape rect;
        rect.setFillColor(sf::Color::Transparent);
        sf::Vector2f size(Qtree.getBoundary().getSize().x, Qtree.getBoundary().getSize().y);
        rect.setSize(size);
        rect.setPosition(Qtree.getBoundary().getStartingPoint().x, Qtree.getBoundary().getStartingPoint().y);
        rect.setOutlineThickness(-1);
        win.draw(rect);
    }
    for(auto &element : Qtree.getElements())
    {
        Circle *Obj = static_cast<Circle*>(element.getObjectPointer());
        if((*Obj).inCollision)
        {
            Collisions++;
        }
        (*Obj).drawTo(win);
    }
    if(Qtree.isDivided())
    {
        for(int i = 0; i < 4; i++)
        {
            qt::QuadTreeNode *node = Qtree.getChild(i);
            drawByQTree(*node);
        }
    }
}

void App::windowLogic(std::stringstream &InfoStream, qt::QuadTreeNode &Qtree)
{   
    InfoStream << "Press SPACE to switch between std::Vector and QuadTree!\n";
    InfoStream << CheckStr << Dduration.count() << '\n';

    //Check collisions
        
    timePoint = std::chrono::system_clock::now();
        
    OverlapPairs.clear();
    (ByVector)? checkByVector() : checkByQuadTree(Qtree);
        
    for(auto &pair: OverlapPairs)
    {
        (*pair.first).inCollision = true;
        (*pair.second).inCollision = true;
    }

    //End clock and claculate duration
    std::chrono::duration<float> CVduration = std::chrono::system_clock::now() - timePoint;
    InfoStream << CheckStr << CVduration.count() << '\n';

    InfoStream << "Checks done: " << CheckMade;
    CheckMade = 0;
        
    InfoStream << "  Circles in collision: " << Collisions; 
    Collisions = 0;
}

void App::windowDrawing(std::stringstream &InfoStream, qt::QuadTreeNode &Qtree)
{
    win.clear();

    timePoint = std::chrono::system_clock::now();
    (ByVector)? drawByVector() : drawByQTree(Qtree);
    Dduration = std::chrono::system_clock::now() - timePoint;
        
    win.draw(ComputationTimeInfo);
        
    win.display();
}


//Public:

App::App()
{
    FontData = CyberFont.read();
    font.loadFromMemory(FontData.data(), FontData.size());
    ComputationTimeInfo.setFont(font);
    ComputationTimeInfo.setCharacterSize(20);
    ComputationTimeInfo.setPosition(0,0);
}

void App::run()
{
    win.create(sf::VideoMode(1280,720), "QuadTree", sf::Style::Close);
    
    //Add circles to vector
    for(int i = 0; i < 200; i++)
    {
        Circle qtC(sf::Vector2f(rand()%1260 + 10, rand()%590 + 130), 3 + rand()%7,i+1);
        qtC.setCollisionColor(sf::Color::Green);
        qtC.setColor(sf::Color::White);
        int speed = rand() % 100;
        qtC.setSpeed(speed + 30);
        qtC.setBoundaryX(10,1270);
        qtC.setBoundaryY(130,710);
        Circles.push_back(qtC);
    }

    while(win.isOpen())
    {
        deltaTime = DeltaTimeClock.restart().asSeconds();
        
        //Handle Events
        sf::Event ev;
        HandleEvents(ev);
        
        //Logic

        //QuadTree update
        qt::QuadTreeNode Qtree(qt::Rectangle(0,120,1280,600),4);
        for(auto &Circle : Circles)
        {
            Circle.inCollision = false;
            Circle.move(deltaTime);
            qt::Element el(Circle.getPosition().x, Circle.getPosition().y, Circle.getID());
            el.setObjectPointer(&Circle);
            Qtree.insert(el);
        }

        //String stream to store inforamtion that is displayed to the screen
        std::stringstream InfoStream;
        
        windowLogic(InfoStream, Qtree);
        
        ComputationTimeInfo.setString(InfoStream.str());
        
        //Drawing

        windowDrawing(InfoStream,Qtree);
    
    }
}

App::~App(){}