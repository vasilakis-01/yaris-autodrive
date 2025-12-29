/*NavigationSystem.hpp*/
#ifndef NAVIGATIONSYSTEM_HPP
#define NAVIGATIONSYSTEM_HPP

#include "SensorReading.hpp"
#include "Utilities.hpp"
#include "SensorFusionEngine.hpp"
#include "Gridworld.hpp"
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

//struct to organize possible navigation system's decisions
struct NavigationDecision {
    bool accelerate = false;
    bool decelerate = false;
    bool turn = false;
    bool stop = false;
    Direction newDirection = Direction::NORTH;
    
    //debug info
    string toString() const {
        string result = "Decision: ";
        if (accelerate) result += "ACCELERATE ";
        if (decelerate) result += "DECELERATE ";
        if (turn) result += "TURN to " + directionToString(newDirection) + " ";
        if (stop) result += "STOP ";
        return result;
    }
};

//Navigation system class
class NavigationSystem {
    private:
        Position currentTarget;
        bool hasTarget = false;
        SensorFusionEngine fusionEngine;
    
        //vector to store last fused readings
        vector<SensorReading> lastFusedReadings;
    
    public:
        //initialize navigation system and fuzion engine
        NavigationSystem(double confidenceThreshold = 0.4): fusionEngine(confidenceThreshold) {}

        //function to set target
        void setCurrentTarget(Position target) {
            currentTarget = target;
            hasTarget = true;
            cout << "[NAV] Target set to: " << target.toString() << endl;
        }
    
        void updateSensorData(const vector<SensorReading>& readings) {
            //update sensors after fusion
            lastFusedReadings = fusionEngine.fuseSensorData(readings);
        
            //debug info
            cout << "[NAV] Memory updated with " << lastFusedReadings.size()<< " fused objects" << endl;
        
            for (const auto& reading : lastFusedReadings) {
                if (reading.confidence > 0.7) { 
                    cout << "  - " << reading.toString() << endl;
                }
            }
        }
        
        //function to make decision
        NavigationDecision makeDecision(Position currentPos, Direction currentDir, GridWorld* world) {
            NavigationDecision decision;
    
            if (!hasTarget) {
                cout << "[NAV] No target set" << endl;
                return decision;
            }
    
            //get the direction to the target
            Direction targetDir = calculateDirectionToTarget(currentPos);
            if (targetDir != currentDir) {
                decision.turn = true;
                decision.newDirection = targetDir;
            }
    
            //get the distance to the target
            int distanceToTarget = currentPos.manhDistance(currentTarget);
            cout << "[NAV] Distance to target: " << distanceToTarget << endl;
    
            //decelerate when approaching the target
            if (distanceToTarget <= 3) { 
                decision.decelerate = true;
                cout << "[NAV] Decelerating: approaching target (distance: " 
                << distanceToTarget << ")" << endl;
            }
    
            //check for obstacles
            checkForHazards(currentPos, currentDir, world, decision);
    
            //if you dont see any obstacles while approaching dont stop
            if (!decision.decelerate && !decision.stop && distanceToTarget > 1) {
                decision.accelerate = true;
            }
    
            //when the car is too close keep the same speed state
            if (distanceToTarget <= 2) {
                decision.accelerate = false;
                decision.decelerate = false;
                cout << "[NAV] Very close to target - maintaining speed" << endl;
            }
    
            //check for stop signs ahead
            if (hasStopSignAhead(currentPos, currentDir)) {
                decision.stop = true;
                decision.decelerate = true;
                cout << "[NAV] STOP sign detected!" << endl;
            }
    
            cout << "[NAV] " << decision.toString() << endl;
            return decision;
        }
    
        //helper function to check the enviroment
        void checkEnvironment(Position carPos, Direction carDir) {
            cout << "\n=== ENVIRONMENT CHECK ===" << endl;
        
            for (const auto& reading : lastFusedReadings) {
                if (reading.confidence > 0.6) {
                    double distance = carPos.manhDistance(reading.position);
                
                    //check for close objects
                    if (distance < 5) {
                        cout << "Nearby: " << reading.objectType << " at distance " << distance 
                         << ", confidence " << reading.confidence << endl;
                    
                        //check for every object that can spawn
                        if (reading.objectType.find("CAR") != string::npos && distance < 3) {
                            cout << "  WARNING: Car too close!" << endl;
                        }
                        if (reading.objectType == "BIKE" && distance < 4) {
                            cout << "  CAUTION: Bike nearby!" << endl;
                        }
                        if (!reading.trafficLightColor.empty()) {
                            cout << "  Traffic light: " << reading.trafficLightColor << endl;
                        }
                        if (!reading.signText.empty()) {
                            cout << "  Sign: " << reading.signText << endl;
                        }
                    }
                }
            }
            cout << "========================\n" << endl;
        }
    
    private:
        //function to calculate direction to target
        Direction calculateDirectionToTarget(Position currentPos) {
            int dx = currentTarget.getX() - currentPos.getX();
            int dy = currentTarget.getY() - currentPos.getY();
    
            //first go to the correct block of x axis and the to the correct block of the y axis
            if (abs(dx) > abs(dy)) {
                return (dx > 0) ? Direction::EAST : Direction::WEST;
            }   else if (abs(dy) > abs(dx)){
            return (dy > 0) ? Direction::NORTH : Direction::SOUTH;
            }  else {
                //keep the same logic if (dx=dy)
                if (dx != 0) {
                    return (dx > 0) ? Direction::EAST : Direction::WEST;
                } else {
                    return (dy > 0) ? Direction::NORTH : Direction::SOUTH;
                }
            }


        }
        //function to check for possible hazards
        void checkForHazards(Position carPos, Direction carDir, GridWorld* world, NavigationDecision& decision) {
            Position moveVec = directionToVector(carDir);
        
            //check for moving objects 3 blocks ahead
            for (int i = 1; i <= 3; i++) {
                Position checkPos(
                    carPos.getX() + moveVec.getX() * i,
                    carPos.getY() + moveVec.getY() * i
                );
            
                //in bounds check
                if (!world->isInside(checkPos)) {
                    decision.decelerate = true;
                    cout << "[NAV] Decelerating: near world boundary" << endl;
                    break;
                }
            
                //check according to sensros data
                for (const auto& reading : lastFusedReadings) {
                    if (reading.position == checkPos && reading.confidence > 0.6) {
                        //for objects ahead
                        if (reading.objectType.find("CAR") != string::npos || 
                            reading.objectType.find("BIKE") != string::npos) {
                            if (i <= 2) {  //2 blocks ahead
                                decision.decelerate = true;
                                cout << "[NAV] Decelerating: obstacle at " << checkPos.toString() << endl;
                            }
                        }
                    
                        //Traffic light check
                        if (!reading.trafficLightColor.empty()) {
                            if (reading.trafficLightColor == "RED" || 
                                reading.trafficLightColor == "YELLOW") {
                                if (i <= 3) {  //3 blocks ahead
                                    decision.decelerate = true;
                                    cout << "[NAV] Decelerating: " << reading.trafficLightColor << " light" << endl;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        //function to operate if a stop sign is ahead
        bool hasStopSignAhead(Position carPos, Direction carDir) {
            Position moveVec = directionToVector(carDir);
        
            //ckeck 5 blocks ahead
            for (int i = 1; i <= 5; i++) {
                Position checkPos(
                    carPos.getX() + moveVec.getX() * i,
                    carPos.getY() + moveVec.getY() * i
                );
                //check for sign's text if it is "STOP"
                for (const auto& reading : lastFusedReadings) {
                    if (reading.position == checkPos && 
                        reading.confidence > 0.7 &&
                        reading.signText == "STOP") {
                        return true;
                    }
                }
            }
            return false;
        }
};

#endif