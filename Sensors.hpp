/*Sensors.hpp*/
#ifndef SENSORS_HPP
#define SENSORS_HPP

#include "SensorReading.hpp"
#include "Utilities.hpp"
#include "Gridworld.hpp"
#include <string>
#include <memory>
#include <vector>

using namespace std;



//basic Sensor class 
class Sensor {
    protected:
        string id;              //sensor's distinct id
        string type;            //sensor's type (lidar,camera,radar)
        double baseAccuracy;    //accuracy (0-1.0)
        int range;              //range of every sensor in the world
        Position position;      //sensor's position
        Direction direction;    //sensor's direction
    
    public:
        //constructor
        Sensor(string sensorId, string sensorType, double accuracy, int rng): id(sensorId), type(sensorType), baseAccuracy(accuracy), range(rng) {}
    
        //destructor
        virtual ~Sensor() {}
    
        // basic virtual function to collect data from every sensor
        virtual vector<SensorReading> sense(Position carPos, Direction carDir, GridWorld* world) = 0;
    
        //getters
        string getId() const { return id; }
        string getType() const { return type; }
        double getBaseAccuracy() const { return baseAccuracy; }
        int getRange() const { return range; }
    
    protected:
        //function to calculate confidence in every measurement
        double calculateConfidence(double distance) const {
            if (distance > range) return 0.0;//check for everything that is in range
        
            //linear improvement in confidence
            double distanceFactor = 1.0 - (distance / range); //basically if an object is closer 
                                                              //it can be identified with more confidence
        
            //noise addition randomnumber from -0.5 to +0.5
            double noise = (rand() % 100 - 50) / 1000.0;
        
            return max(0.0, min(1.0, baseAccuracy * distanceFactor + noise)); //last calculation of confidence according to all factos
        }
    
        //helper function to calculate Manhattan distance
        double manhattanDistance(Position pos1, Position pos2) const {
            return abs(pos1.getX() - pos2.getX()) + abs(pos1.getY() - pos2.getY());
        }
};

//Lidar sensor
class LidarSensor : public Sensor {
    private:
        int fieldOfView;  // fild of view is a 9x9 square
    
    public:
        LidarSensor(string id): Sensor(id, "LIDAR", 0.99, 9),fieldOfView(9) {}  //during construction we give it 99% confidence and 9 blocks range
        
        //function to collect info from the lidar sensor
        vector<SensorReading> sense(Position carPos, Direction carDir, GridWorld* world) override {
            vector<SensorReading> readings;
        
            int halfView = 4; //lidar's range is an 9x9 square so its halfview is 9/2=4 so it sees from -4 to +4
        
            for (int dx = -halfView; dx <= halfView; dx++) { //on x axis -4 to +4
                for (int dy = -halfView; dy <= halfView; dy++) { //on y axis -4 to +4
                    Position checkPos(carPos.getX() + dx, carPos.getY() + dy);
                
                    //exclude the self driving car it self
                    if (dx == 0 && dy == 0) continue;
                
                    //check positions that are in bounds of the world
                    if (!world->isInside(checkPos)) continue;
                
                    //look for an object
                    WorldObject* obj = world->getObjectAt(checkPos);
                    if (obj != nullptr) {
                        //calculate the dinstance to the object
                        double distance = abs(dx)+abs(dy);
                    
                        //Lidar sensor cant see traffic light's color or sign's text
                        SensorReading reading;
                        reading.objectId = obj->getId();
                        reading.objectType = obj->getType();
                        reading.position = checkPos;
                        reading.distance = distance;
                        reading.confidence = calculateConfidence(distance);
                        readings.push_back(reading);
                    }
                }
            }
        
            return readings;
        }
};

//Radar sensor class
class RadarSensor : public Sensor {
    public:
        RadarSensor(string id): Sensor(id, "RADAR", 0.95, 12) {}  //during contruction we give it 95% confidence and 12 blocks range
    
        vector<SensorReading> sense(Position carPos, Direction carDir,GridWorld* world) override {
            vector<SensorReading> readings;
        
            //radar can only see in the same direction that the car faces
            Position moveVec = directionToVector(carDir);
        
            //sense across all range
            for (int i = 1; i <= range; i++) {
                Position checkPos(
                    carPos.getX() + moveVec.getX() * i,
                    carPos.getY() + moveVec.getY() * i
                );
            
                //in bounds check
                if (!world->isInside(checkPos)) break;
            
                //check for a moving object
                WorldObject* obj = world->getObjectAt(checkPos);
                if (obj != nullptr && obj->isMoving()) {

                    double distance = manhattanDistance(carPos, checkPos);
                
                    SensorReading reading;
                    reading.objectId = obj->getId();
                    reading.objectType = obj->getType();
                    reading.position = checkPos;
                    reading.distance = distance;
                    reading.confidence = calculateConfidence(distance);
                
                    //Radar sensor can sense object's speed and direction
                    MovingObject* movingObj = dynamic_cast<MovingObject*>(obj);
                    if (movingObj != nullptr) {
                        reading.speed = movingObj->getSpeed();
                        reading.direction = movingObj->getDirection();
                    }
                
                readings.push_back(reading);
                }
            }
        
            return readings;
        }
};

//Camera sensor class
class CameraSensor : public Sensor {
    private:
        int fovWidth;  //width of fiels of view

    public:
        CameraSensor(string id): Sensor(id, "CAMERA", 0.87, 7),fovWidth(7) {}  // 87% confidence, 7 blocks -> 7x7 range
    
        vector<SensorReading> sense(Position carPos, Direction carDir, GridWorld* world) override {
            vector<SensorReading> readings;
        
            //camera can see in a square on both axis for -3 to +3 (-3<=x<=+3),(-3<=y<=+3)
            int halfWidth = fovWidth / 2;
        
            for (int dx = -halfWidth; dx <= halfWidth; dx++) {
                for (int dy = -halfWidth; dy <= halfWidth; dy++) {
                    //optimize regarding the direction of the car
                    int checkX, checkY;
                    if (carDir == Direction::NORTH) { //NORTH
                        checkX = carPos.getX() + dx;
                        checkY = carPos.getY() + dy;
                    } else if (carDir == Direction::EAST) {//EAST
                        checkX = carPos.getX() + dy;
                        checkY = carPos.getY() - dx;
                    } else if (carDir == Direction::SOUTH) {//SOUTH
                        checkX = carPos.getX() - dx;
                        checkY = carPos.getY() - dy;
                    } else { //WEST
                        checkX = carPos.getX() - dy;
                        checkY = carPos.getY() + dx;
                    }
                
                    Position checkPos(checkX, checkY);
                
                    //exclude the car it self
                    if (checkX == carPos.getX() && checkY == carPos.getY()) continue;
                
                    //inbounds check
                    if (!world->isInside(checkPos)) continue;
                
                    //look for an object
                    WorldObject* obj = world->getObjectAt(checkPos);
                    if (obj != nullptr) {
                        
                        double distance = manhattanDistance(carPos, checkPos);

                        //update readings with the data collected
                        SensorReading reading;
                        reading.objectId = obj->getId();
                        reading.objectType = obj->getType();
                        reading.position = checkPos;
                        reading.distance = distance;
                        reading.confidence = calculateConfidence(distance);
                    
                        //Camera can see traffic light's color
                        TrafficLight* light = dynamic_cast<TrafficLight*>(obj);
                        if (light != nullptr) {
                            reading.trafficLightColor = light->getColor();
                        }
                    
                        //Camera can se sign's text
                        StreetSign* sign = dynamic_cast<StreetSign*>(obj);
                        if (sign != nullptr) {
                            reading.signText = sign->getText();
                        }
                    
                        readings.push_back(reading);
                    }
                }
            }
        
            return readings;
        }
};

#endif