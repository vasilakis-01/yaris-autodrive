#ifndef GRIDWORLD_HPP
#define GRIDWORLD_HPP

#include <vector>
#include <iostream>
#include "Utilities.hpp"
#include "WorldObject.hpp"


//gridworld class
class GridWorld {
private:
    int width, height; //world's size
    std::vector<std::vector<WorldObject*>> grid; //vector that consists of pointers tha point to all the objects in the world
    
public:
    //contructor
    GridWorld(int w = 40, int h = 40) : width(w), height(h) {
        grid.resize(height); //world resize according to the inputs given from the user
        for (int y = 0; y < height; y++) {
            grid[y].resize(width, nullptr);
        }
    }
    
    //destructor
    ~GridWorld() {
        clear();
    }
    
    //during destruction of the world set all the object pointers to null to avoid memory leaks
    void clear() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (grid[y][x] != nullptr) {
                    delete grid[y][x]; //memory deallocation
                    grid[y][x] = nullptr; // set pointer->NULL
                }
            }
        }
    }
    
    //function to add an object in the world
    bool addObject(WorldObject* obj) {

        Position pos = obj->getPosition(); //get objects position
        
        //check if its in bounds
        if (pos.getX() < 0 || pos.getX() >= width || 
            pos.getY() < 0 || pos.getY() >= height) {
            return false;
        }
        
        //initiate it in the world
        grid[pos.getY()][pos.getX()] = obj;
        return true;
    }
    
    //function to update the world according to the ticks
    void update(int tick) {
        
        //create a vector of pointers that point to objects that changed position
        std::vector<std::pair<WorldObject*, Position>> movedObjects;
        
        //update all objects according to their type (the specific update function of each object will operate)
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                //run through the world and update each object
                WorldObject* obj = grid[y][x];
                if (obj != nullptr) {
                    Position oldPos = obj->getPosition();
                    obj->update(tick);
                    Position newPos = obj->getPosition();
                    
                    //if it was a moving object and it changed position store it to the moved object vector
                    if (oldPos.getX() != newPos.getX() || oldPos.getY() != newPos.getY()) {
                        movedObjects.push_back({obj, oldPos});
                    }
                }
            }
        }
        
        //do the movement of the object
        for (auto& pair : movedObjects) {
            WorldObject* obj = pair.first;
            Position oldPos = pair.second;
            Position newPos = obj->getPosition();
            
            //clear old position
            if (grid[oldPos.getY()][oldPos.getX()] == obj) {
                grid[oldPos.getY()][oldPos.getX()] = nullptr;
            }
            
            //add the object to its new posiiton and check if the position is valid
            if (newPos.getX() >= 0 && newPos.getX() < width &&
                newPos.getY() >= 0 && newPos.getY() < height) {
                grid[newPos.getY()][newPos.getX()] = obj;
            } else {
                //if the new position isnt valid the object went out of bound and is deleted
                delete obj;
            }
        }
    }
    
    //function to visualize the world
    void show() {
        std::cout << "\nGrid World (" << width << "x" << height << "):\n";
        
        for (int y = height - 1; y >= 0; y--) {
            std::cout << "Row " << y << ": ";
            for (int x = 0; x < width; x++) {
                if (grid[y][x] == nullptr) {
                    std::cout << ". ";
                } else {
                    std::cout << grid[y][x]->getGlyph() << " ";
                }
            }
            std::cout << std::endl;
        }
    }
    
    //getters
    WorldObject* getObjectAt(int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return grid[y][x];
        }
        return nullptr;
    }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif // GRIDWORLD_HPP