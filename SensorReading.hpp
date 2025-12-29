/*SensorReading.hpp*/
#ifndef SENSORREADING_HPP
#define SENSORREADING_HPP

#include "Utilities.hpp"
#include <string>
#include <memory>
#include <vector>

using namespace std;
//struct to better organize the data that the sensors can read
struct SensorReading {
    string objectId;  //object's id
    string objectType; //obkect's type
    Position position; // object's position
    double distance; // distance from the car to the object
    double confidence; // sensor's confidence
    double speed; // object's speed
    Direction direction; // object's direction
    string signText; //text from a sign
    string trafficLightColor; //color from a traffic light
    
    SensorReading() : distance(0.0), confidence(0.0), speed(0.0),direction(Direction::NORTH) {}
    
    //initialization of the struct with default values
    SensorReading(string id, string type, Position pos, double dist, double conf): objectId(id), objectType(type), position(pos), 
                                                                                   distance(dist), confidence(conf), speed(0.0),
                                                                                   direction(Direction::NORTH) {}
    //function to print the data collected
    string toString() const {
        string result = "Reading[" + objectId + "]: type->" + objectType + 
                       ", pos->" + position.toString() + 
                       ", dist->" + to_string(distance) + 
                       ", conf->" + to_string(confidence);
        
        if (!signText.empty()) result += ", sign->" + signText;
        if (!trafficLightColor.empty()) result += ", light->" + trafficLightColor;
        if (speed > 0) result += ", speed->" + to_string(speed);
        
        return result;
    }
};

#endif