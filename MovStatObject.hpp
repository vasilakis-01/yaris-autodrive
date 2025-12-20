#ifndef MOVSTATOBJECT_HPP
#define MOVSTATOBJECT_HPP

#include "WorldObject.hpp"

//static object funtion
class StaticObject : public WorldObject{
    public:
        StaticObject(std::string type, char glyph, Position pos) : WorldObject(type, glyph, pos) {}
        
        bool isStatic() const override {return true;}
        bool isMoving() const override {return false;}
        void update(int tick) override {
            (void)tick;
        }
};

//moving object class
class MovingObject : public WorldObject{
    protected: 
        //speed,direction
        Direction direction;
        int speed;
    
    public:
        MovingObject(std::string type, char glyph, Position pos, Direction dir, int s) 
            : WorldObject(type, glyph, pos), direction(dir), speed(s) {}

        //move to each dir function
        void update(int tick) override {  // ΠΡΟΣΘΗΚΗ: override
            Position movePos = directionToVector(direction);
            int newX = position.getX() + movePos.getX() * speed;
            int newY = position.getY() + movePos.getY() * speed;
            position = Position(newX, newY);
            (void)tick;
        }

        bool isStatic() const override { return false; }
        bool isMoving() const override { return true; }
    
        //getters
        Direction getDirection() const { return direction; }
        int getSpeed() const { return speed; }
    
        //setters
        void setDirection(Direction newDir) { direction = newDir; }
        void setSpeed(int newSpeed) { speed = newSpeed; }
};

#endif