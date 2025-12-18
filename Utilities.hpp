#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>

//position class
class Position {
private:
    int x, y;
    
public:
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    
    //getters
    int getX() const { return x; }
    int getY() const { return y; }
    
    //setters
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    
    //set operator to compare positions for easier use
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
    
    //manhattan distance
    int manhDistance(const Position& other) const {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }
    
    //move function
    Position move(int dx, int dy) const {
        return Position(x + dx, y + dy);
    }
    
    //in bounds check
    bool isWithinBounds(int minX, int maxX, int minY, int maxY) const {
        return (x >= minX && x < maxX && y >= minY && y < maxY);
    }
    
    //print for debugging (remove later)
    std::string toString() const {
        return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
    }
};

class Random {
    public:
        Random(int seed = -1) {
            if (seed == -1) {
            //set the deafault seed if one is not provided
            srand(time(NULL));
            } else {
            //set the given seed
            srand(seed);
        }
        }
    
        //function to get random number from min to max
        int getInt(int min, int max) {
            // rand() % (max - min + 1) + min
            return rand() % (max - min + 1) + min;
    }
};


//direction class
enum class Direction {
    NORTH,
    EAST, 
    SOUTH,
    WEST
};


//print for debugging might remove later
std::string directionToString(Direction dir) {
    switch(dir) {
        case Direction::NORTH: return "NORTH";
        case Direction::EAST:  return "EAST";
        case Direction::SOUTH: return "SOUTH";
        case Direction::WEST:  return "WEST";
        default: return "UNKNOWN";
    }
}

//move to direction
Position directionToVector(Direction dir) {
    switch(dir) {
        case Direction::NORTH: return Position(0, 1);
        case Direction::EAST:  return Position(1, 0);
        case Direction::SOUTH: return Position(0, -1);
        case Direction::WEST:  return Position(-1, 0);
        default: return Position(0, 0);
    }
}

Direction randomDirection(Random& rng) {
    int dir = rng.getInt(0, 3);  // 0, 1, 2, 3
    return static_cast<Direction>(dir);
}


#endif // UTILITIES_HPP