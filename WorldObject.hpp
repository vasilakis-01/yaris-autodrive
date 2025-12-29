/*WorldObject.hpp*/
#ifndef WORLDOBJECT_HPP
#define WORLDOBJECT_HPP

#include "Utilities.hpp"
#include <string>

//basic class for all objects
class WorldObject {
protected:
    std::string id;      //specific id for each object
    std::string type;    //object's type
    char glyph;         //object's symbol for visualization
    Position position;   //object's position
    
public:
    //constructor
    WorldObject(std::string objType, char objGlyph, Position pos)
        : type(objType), glyph(objGlyph), position(pos) 
    {
        //id generator
        static int counter = 1;
        id = type + std::to_string(counter++);
    }
    
    //virtual destructor
    virtual ~WorldObject() {}

    //update fucntion will differenciate in the objects
    virtual void update(int tick) = 0;
    
    
    //is static object check
    virtual bool isStatic() const = 0;
    
    //is moving object check
    virtual bool isMoving() const = 0;
    
    //getters
    std::string getId() const { return id; }
    std::string getType() const { return type; }
    char getGlyph() const { return glyph; }
    Position getPosition() const { return position; }
    
    //seters
    void setPosition(Position newPos) { position = newPos; }
    void setGlyph(char newGlyph) { glyph = newGlyph; }
};

#endif