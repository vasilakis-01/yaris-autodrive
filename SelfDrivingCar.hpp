/*SelfDrivingCar.hpp*/
#ifndef SELFDRIVINGCAR_HPP
#define SELFDRIVINGCAR_HPP

#include <iostream>
#include <vector> 
#include <memory>
#include <string>
#include "MovingObjects.hpp"
#include "Gridworld.hpp"
#include "Utilities.hpp"
#include "Sensors.hpp"
#include "SensorFusionEngine.hpp"
#include "NavigationSystem.hpp"
#include "SensorReading.hpp"

using namespace std;

//speed state class
enum class SpeedState {
    STOPPED = 0,
    HALF_SPEED = 1, //1 position per tick
    FULL_SPEED = 2 // 2 positions per tick
};

class SelfDrivingCar : public MovingObject {
    private:
        SpeedState speedState; //speedstate
        Direction direction; //car direction
        GridWorld* world; //pointer to the world
    
        //sensors
        vector<unique_ptr<Sensor>> sensors;
    
        //navigation system
        unique_ptr<NavigationSystem> navigationSystem;

        //gps targets
        vector<Position> gpsTargets;
        size_t currentGpsTargetIndex;

        //sensor fusion engine
        unique_ptr<SensorFusionEngine> fusionEngine;

    public:
        //constructor
        SelfDrivingCar(Position startPos, Direction dir, GridWorld* wrldPointer): MovingObject("CAR", '@', startPos, dir, 0),
                                                                        speedState(SpeedState::STOPPED),
                                                                        direction(dir),world(wrldPointer),currentGpsTargetIndex(0){
            cout << "[+VEHICLE:" << getId() << "] Self driving car initialized at " << startPos.toString() << ", heading " 
                << directionToString(dir) << endl;
        
            //create a new fuse sesnor engine
            fusionEngine.reset(new SensorFusionEngine(0.4)); // 40% threshold
        
            //add sensors
            addDefaultSensors();
        
            //create a new navigation system
            navigationSystem.reset(new NavigationSystem(0.4));
        
            cout << "[SYSTEM] Car equipped with 3 sensors and navigation system" << endl;
        }

        //destructor
        ~SelfDrivingCar() {
            cout << "[-CAR:" << getId() << "] Self driving car finished its journey" << endl;
        
            cout << "[STATS] Final position: " << getPosition().toString() << endl;
            cout << "[STATS] GPS targets reached: " << currentGpsTargetIndex << "/" << gpsTargets.size() << endl;
        }

        void update(int tick) override {
            cout << "\n[TICK " << tick << "] Car " << getId() << " updating..." << endl;
        
            //collect sensor data
            vector<SensorReading> readings = collectSensorData();
        
            //fuse collected data
            vector<SensorReading> fusedReadings = fuseSensorData(readings);
        
            //update navi system
            syncNavigationSystem(fusedReadings);
        
            //make a move
            executeMovement();
        
            //display car's condition (speedstate,position)
            displayStatus();
        
            (void)tick;
        }

        //helper functions
        bool isStatic() const override { return false; }
        bool isMoving() const override { return true; }


        //funciton to set the gps route
        void setGPSTargets(const vector<Position> &targets) {
            gpsTargets = targets;
            currentGpsTargetIndex = 0;

            if(!gpsTargets.empty()) {
                cout << "[GPS:" << getId() << "] GPS route set with " << gpsTargets.size() << " targets" << endl;
            
                //print all the targets
                for (size_t i = 0; i < gpsTargets.size(); i++) {
                    cout << "  Target " << (i+1) << ": " << gpsTargets[i].toString() << endl;
                }
            
                //set the first target to the navigation system
                if (navigationSystem) {
                    navigationSystem->setCurrentTarget(gpsTargets[currentGpsTargetIndex]);
                }
            } else {
                cout << "[GPS:" << getId() << "] Warning: No GPS targets provided!" << endl;
            }
        }   

        //add sensor function
        void addSensor(unique_ptr<Sensor> sensor) {
            sensors.push_back(move(sensor));
            cout << "[SENSOR] Added " << sensors.back()->getType() << " sensor: " << sensors.back()->getId() << endl;
        }

        //function to initiate a simple navigation system if one hasn't been created
        void setSimpleNavigation() {
            if (!navigationSystem) {
                navigationSystem = unique_ptr<NavigationSystem>(new NavigationSystem(0.4));
            }
        
            if (!gpsTargets.empty()) {
                navigationSystem->setCurrentTarget(gpsTargets[currentGpsTargetIndex]);
                cout << "[NAV] Navigation targeting: " << gpsTargets[currentGpsTargetIndex].toString() << endl;
            }
        }

        //add our three sensors(lidrar,radar,camera)
        void addDefaultSensors() {
            //Lidar Sensor
            sensors.push_back(unique_ptr<Sensor>(new LidarSensor(getId() + "_LIDAR")));
            cout << "[+LDAR:" << sensors.back()->getId() << "] Lidar sensor ready"  << endl;
        
            //Radar Sensor
            sensors.push_back(unique_ptr<Sensor>(new RadarSensor(getId() + "_RADAR")));
            cout << "[+RADAR:" << sensors.back()->getId() << "] Radar sensor ready" << endl;
        
            //Camera Sensor
            sensors.push_back(unique_ptr<Sensor>(new CameraSensor(getId() + "_CAMERA")));
            cout << "[+CAMERA:" << sensors.back()->getId() << "] Camera sensor ready" << endl;
        
            cout << "[SYSTEM] Total sensors: " << sensors.size() << endl;
        }

        //getters
        SpeedState getSpeedState() const { return speedState; }
        Direction getDirection() const { return direction; }
    
        Position getCurrentTarget() const {
            if (currentGpsTargetIndex < gpsTargets.size()) {
                return gpsTargets[currentGpsTargetIndex];
            }
            return getPosition();
        }
    
        size_t getCurrentTargetIndex() const { return currentGpsTargetIndex; }
        size_t getTotalTargets() const { return gpsTargets.size(); }
    
        //info returns for debugging 
        string getStatus() const {
            string status = "Car " + getId() + ": ";
            status += "Pos->" + getPosition().toString() + ", ";
            status += "Dir->" + directionToString(direction) + ", ";
        
            switch(speedState) {
                case SpeedState::STOPPED: status += "Speed=STOPPED"; break;
                case SpeedState::HALF_SPEED: status += "Speed=HALF"; break;
                case SpeedState::FULL_SPEED: status += "Speed=FULL"; break;
            }
        
            status += ", Target=" + getCurrentTarget().toString();
            status += " (" + to_string(currentGpsTargetIndex+1) + "/" + to_string(gpsTargets.size()) + ")";
        
            return status;
        }
    
    private:
        //collection of all sensors
        vector<SensorReading> collectSensorData() {
            vector<SensorReading> allReadings;
        
            if (sensors.empty()) {
                cout << "[SENSOR] Warning: No sensors available!" << endl;
                return allReadings;
            }
        
            int totalDetections = 0; //detections counter
            
            //start detection with each sensor
            for (const auto& sensor : sensors) {
                vector<SensorReading> sensorReadings = sensor->sense(getPosition(), direction, world);
            
                totalDetections += sensorReadings.size();   
            
                //add all detections
                allReadings.insert(allReadings.end(),sensorReadings.begin(), sensorReadings.end());
            
                //debug info
                cout << "[SENSOR] " << sensor->getType() << " detected " << sensorReadings.size() << " objects" << endl;
            }
        
            cout << "[SENSOR] Total raw detections: " << totalDetections << " from " << sensors.size() << " sensors" << endl;
        
            return allReadings;
        }
    
        //function to fuse all data together
        vector<SensorReading> fuseSensorData(const vector<SensorReading>& readings) {
        if (!fusionEngine) {
            fusionEngine.reset(new SensorFusionEngine(0.4));
            }
        
            vector<SensorReading> fused = fusionEngine->fuseSensorData(readings);
        
            cout << "[FUSION] Fused " << readings.size()<< " readings into " << fused.size() << " objects" << endl;
        
            //show as fuzed all the objects detected with confidence>70%
            for (const auto& reading : fused) {
                if (reading.confidence > 0.7) {
                    cout << "  High confidence: " << reading.objectType << " at " << reading.position.toString() 
                         << " (conf: " << reading.confidence << ")" << endl;
                }
            }
        
            return fused;
        }
    
        //sync navi system
        void syncNavigationSystem(const vector<SensorReading>& readings) {
            if (navigationSystem) {
                navigationSystem->updateSensorData(readings);
            
                //debug info
                navigationSystem->checkEnvironment(getPosition(), direction);
            } else {
                cout << "[NAV] Warning: No navigation system!" << endl;
            }
        }
    
        //function to decide the following movement
        void executeMovement() {
            if (!navigationSystem) {
                cout << "[MOVEMENT] No navigation system - staying put" << endl;
                return;
            }
    
            //navi system's decision
            NavigationDecision decision = navigationSystem->makeDecision(getPosition(), direction, world);
    
            cout << "[DECISION] " << decision.toString() << endl;
    
            //turn
            if (decision.turn) {
                //check if a turn is possible
                Position testPos = getPosition();
                Position moveVec = directionToVector(decision.newDirection);
                testPos = Position(testPos.getX() + moveVec.getX(),testPos.getY() + moveVec.getY());
        
                //in bound check
                if (world->isInside(testPos)) {
                    WorldObject* obj = world->getObjectAt(testPos);
                    if (obj != nullptr && obj != this) {
                        cout << "[NAV] Cannot turn " << directionToString(decision.newDirection)<< " - obstacle at " << testPos.toString() << endl;
                        decision.turn = false; //cancel the turn if it cannot be done
                
                    //pick a diff direction to make a turn to
                    if (decision.newDirection == Direction::NORTH) {
                        decision.newDirection = Direction::EAST;
                    } else if (decision.newDirection == Direction::EAST) {
                        decision.newDirection = Direction::SOUTH;
                    } else if (decision.newDirection == Direction::SOUTH) {
                        decision.newDirection = Direction::WEST;
                    } else {
                        decision.newDirection = Direction::NORTH;
                    }
                    cout << "[NAV] Trying alternative direction: " << directionToString(decision.newDirection) << endl;
                    }
                }
            }
    
            //stop
            if (decision.stop) {
                speedState = SpeedState::STOPPED;
                cout << "[CAR:" << getId() << "] Emergency stop!" << endl;
            } else if (decision.decelerate) { //decelerate
                decelerate();
            } else if (decision.accelerate) { //accelerate
                accelerate();
            }
    
            //turn only if the decision is to turn and there is an availiable direction
            if (decision.turn && decision.newDirection != direction) {
                turn(decision.newDirection);
            }
    
            //move forward
            if (speedState != SpeedState::STOPPED) {
                moveForward();
            } else {
                cout << "[CAR:" << getId() << "] Staying stopped" << endl;
            }
            //check if target is reached
            checkTargetReached();
        }
    
        //accelerate function
        void accelerate() {
            if (speedState == SpeedState::STOPPED) {
                speedState = SpeedState::HALF_SPEED;
                cout << "[CAR:" << getId() << "] Accelerating to HALF_SPEED" << endl;
            } else if (speedState == SpeedState::HALF_SPEED) {
                speedState = SpeedState::FULL_SPEED;
                cout << "[CAR:" << getId() << "] Accelerating to FULL_SPEED" << endl;
            }
        }
    
        //decelerate function
        void decelerate() {
            if (speedState == SpeedState::FULL_SPEED) {
                speedState = SpeedState::HALF_SPEED;
                cout << "[CAR:" << getId() << "] Decelerating to HALF_SPEED" << endl;
            } else if (speedState == SpeedState::HALF_SPEED) {
                speedState = SpeedState::STOPPED;
                cout << "[CAR:" << getId() << "] Decelerating to STOPPED" << endl;
            }
        }
    
        //turn function
        void turn(Direction newDirection) {
            cout << "[CAR:" << getId() << "] Turning from "<< directionToString(direction) << " to " 
                << directionToString(newDirection) << endl;
        
            direction = newDirection;
        }

    //funtion to try to avoid obstacle
    void tryToAvoidObstacle() {
        cout << "[AVOID] Attempting to avoid obstacle..." << endl;
    
        //make a list with availiable direction to resort to,if needed
        vector<Direction> possibleDirections;
    
        //set direction priotiry (North->Eas->South->West)
        if (direction == Direction::NORTH) {
            possibleDirections = {Direction::EAST, Direction::NORTH, Direction::WEST, Direction::SOUTH};
        } else if (direction == Direction::EAST) {
            possibleDirections = {Direction::SOUTH, Direction::EAST, Direction::NORTH, Direction::WEST};
        } else if (direction == Direction::SOUTH) {
            possibleDirections = {Direction::WEST, Direction::SOUTH, Direction::EAST, Direction::NORTH};
        } else {
            possibleDirections = {Direction::NORTH, Direction::WEST, Direction::SOUTH, Direction::EAST};
        }
    
        //try every direction
        for (Direction testDir : possibleDirections) {
            Position moveVec = directionToVector(testDir);
            Position testPos = Position(getPosition().getX() + moveVec.getX(),getPosition().getY() + moveVec.getY());
        
            //in bound check to the new direction
            if (world->isInside(testPos) && world->getObjectAt(testPos) == nullptr) {
                cout << "[AVOID] Changing direction to " << directionToString(testDir)<< " to avoid obstacle" << endl;
                direction = testDir;
                return;
            }
        }
    
        //if all directions are occupied stop
        cout << "[AVOID] All directions blocked! Staying stopped." << endl;
        speedState = SpeedState::STOPPED;
    }
    
    //move to the same direction with the same speed
    void moveForward() {
        int steps = 0; //steps counter

        switch (speedState) {
            case SpeedState::STOPPED: steps = 0; break;
            case SpeedState::HALF_SPEED: steps = 1; break;
            case SpeedState::FULL_SPEED: steps = 2; break;
        }

        cout << "[MOVEMENT] Attempting to move " << steps << " step(s)" << endl;

        for (int i = 0; i < steps; i++) {
            //make a move to the same direction
            Position moveVec = directionToVector(direction);
            Position newPos = Position(getPosition().getX() + moveVec.getX(),getPosition().getY() + moveVec.getY());
        
            //in bound check
            if (!world->isInside(newPos)) {
                cout << "[WARNING] Out of bounds at " << newPos.toString()<< "! Stopping." << endl;
                speedState = SpeedState::STOPPED;
                return;
            }
        
            //obstacle check
            WorldObject* obj = world->getObjectAt(newPos);
            if (obj != nullptr && obj != this) {
                cout << "[COLLISION] Obstacle detected: " << obj->getId()<< " (" << obj->getType() << ") at " << newPos.toString() 
                     << "! Stopping." << endl;
            
                //if obstacle found try to avoid it
                tryToAvoidObstacle();
                return;
            }
        
            //try to move in the world
            if (world->moveObjectTo(this, newPos)) {
                cout << "[MOVEMENT] Successfully moved to " << newPos.toString() << endl;
            
                //check if target is reached
                checkTargetReached();
            } else {
                    cout << "[ERROR] Failed to move to " << newPos.toString() << " - position might be occupied" << endl;
            
                //if failed to move to new position again try to avoid the obstacle
                tryToAvoidObstacle();
                return;
            }
        }
    }

    
    //check if we reached gps target
    void checkTargetReached() {
        if (currentGpsTargetIndex >= gpsTargets.size()) {
            cout << "[GPS] All targets completed" << endl;
            return;
        }
        
        Position currentPos = getPosition();
        Position target = gpsTargets[currentGpsTargetIndex];
        int distance = currentPos.manhDistance(target);
        
        cout << "[GPS] Distance to target " << (currentGpsTargetIndex + 1) 
             << ": " << distance << " units" << endl;
        
        if (distance <= 1) { //we reach the target in a 2 distance radius
            cout << "[GPS] Reached target " << (currentGpsTargetIndex + 1) 
                 << " at " << target.toString() << endl;
            
            currentGpsTargetIndex++; //go to the next target
            
            //set next gps target
            if (currentGpsTargetIndex < gpsTargets.size()) {
                if (navigationSystem) {
                    navigationSystem->setCurrentTarget(gpsTargets[currentGpsTargetIndex]);
                }
                cout << "[GPS] Next target: " << gpsTargets[currentGpsTargetIndex].toString() 
                     << " (" << (currentGpsTargetIndex + 1) << "/" << gpsTargets.size() << ")" << endl;
            } else {
                cout << "[GPS] All " << gpsTargets.size() << " targets reached!" << endl;
                speedState = SpeedState::STOPPED; //stop when all targets reached
            }
        }
    }
    
    //display car status
    void displayStatus() {
        cout << "[STATUS] " << getStatus() << endl;
        
        //speedstate
        string speedBar;
        switch(speedState) {
            case SpeedState::STOPPED: speedBar = "[STOPPED]"; break;
            case SpeedState::HALF_SPEED: speedBar = "[HALF]"; break;
            case SpeedState::FULL_SPEED: speedBar = "[FULL]"; break;
        }
        
        cout << "[SPEED] " << speedBar << endl;
        //keep track of the journey of the car
        if (currentGpsTargetIndex < gpsTargets.size()) {
            Position target = gpsTargets[currentGpsTargetIndex];
            int distance = getPosition().manhDistance(target);
            int maxDistance = 50;
            //print the progress to reaching the tartget
            int progress = min(100, max(0, 100 - (distance * 100 / maxDistance)));
            to_string(progress) + "%";
            
            cout << "[PROGRESS] Target " << (currentGpsTargetIndex + 1)<< " (" << distance << " units)" << endl;
        }
    }
};

#endif
