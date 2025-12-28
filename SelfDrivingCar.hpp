#ifndef SELFDRIVINGCAR_HPP
#define SELFDRIVINGCAR_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "MovingObjects.hpp"
#include "Gridworld.hpp"
#include "Utilities.hpp"

using namespace std;

//speed state class
enum class SpeedState {
    STOPPED = 0,
    HALF_SPEED = 1, //1 position per tick
    FULL_SPEED = 2 // 2 positons per tick
};


class Sensor;
class SensorReading;
class NavigationSystem;
class NavigationDecision;


class SelfDrivingCar : public MovingObject {
private:
    SpeedState speedState;
    Direction direction;
    GridWorld* world;
    //sensors
    vector<unique_ptr<Sensor>> sensors;
    //navigation system
    unique_ptr<NavigationSystem> navigationSystem;

    //gps targets
    vector<Position> gpsTargets;
    size_t currentGpsTargetIndex;

public:
    SelfDrivingCar(Position startPos,Direction dir, GridWorld* wrldPointer): MovingObject("CAR",'@',startPos,dir,0),speedState(SpeedState::STOPPED),direction(dir),
                    world(wrldPointer),currentGpsTargetIndex(0){
                        cout << "Self driving car initialized" <<endl;
                    }

    ~SelfDrivingCar() {
        cout << "Self driving car finished it's journey"<<endl;
    }

    void update(int tick) override {
        vector<SensorReading> readings = collectSensorData();

        syncNavigationSystem(readings);
        
        executeMovement();

        (void)tick;
    }

    bool isStatic() const override { return false; }
    bool isMoving() const override { return true; }

    void setGPStargets(const vector<Position> &targets) {
        gpsTargets = targets;
        currentGpsTargetIndex = 0;

        if(!gpsTargets.empty()) {
            cout <<"GPS route set with "<<gpsTarget.size()<<"targets"<<endl;
        }
    }

    void addSensor(unique_ptr<Sensor> sensor) {
        sensors.push_back(move(sensor));
    }

    void setSimpleNavigation() {
        navigationSystem = make_unique<NavigationSystem>();
        if (!gpsTargets.empty()) {
            navigationSystem->setCurrentTarget(gpsTargets[currentGpsTargetIndex]);
        }
    }

    //getters
    SpeedState getSpeedState() const { return speedState; }
    Direction getDirection() const { return direction; }
    Position getCurrentTarget() const {
        if (currentTargetIndex < gpsTargets.size()) {
            return gpsTargets[currentGpsTargetIndex];
        }
        return getPosition();
    }
    
private:

    vector<SensorReading> collectSensorData() {
        vector<SensorReading> allReadings;

        //keno gia twra

        return allReadings;
    }

    void syncNavigationSystem(const vector<SensorReading>& readings) {
        if (navigationSystem) {
            navigationSystem->updateSensorData(readings);
        }
    }

    void executeMovement() {
        if (!navigationSystem) return;
        
        //navigation decision
        NavigationDecision decision = navigationSystem->makeDecision(getPosition(), direction, world);
        
        //decision to decelerate or accelerate
        if (decision.decelerate) {
            decelerate();
        } else if (decision.accelerate) {
            accelerate();
        }
        
        //decision to turn
        if (decision.turn && decision.newDirection != direction) {
            turn(decision.newDirection);
        }
        
        //else continue
        moveForward();
        
        //and lastly check if the gps target is reached
        checkTargetReached();
    }
    
    //accelerate function
    void accelerate() {
        if (speedState == SpeedState::STOPPED) {
            speedState = SpeedState::HALF_SPEED;
            cout << "Accelerating to HALF_SPEED" << endl;
        } else if (speedState == SpeedState::HALF_SPEED) {
            speedState = SpeedState::FULL_SPEED;
            cout << "Accelerating to FULL_SPEED" << endl;
        }
    }
    
    //decelarate fucntion
    void decelerate() {
        if (speedState == SpeedState::FULL_SPEED) {
            speedState = SpeedState::HALF_SPEED;
            cout <<"Decelerating to HALF_SPEED" << endl;
        } else if (speedState == SpeedState::HALF_SPEED) {
            speedState = SpeedState::STOPPED;
            cout <<"Decelerating to STOPPED" << endl;
        }
    }
    
    //turn function
    void turn(Direction newDirection) {
        direction = newDirection;
        cout <<"Turning to " << directionToString(newDirection) << endl;
    }
    
    //move to the same direction with the same speed
    void moveForward() {
        int steps = 0;

        switch (speedState) {
            case SpeedState::STOPPED: steps = 0; break;
            case SpeedState::HALF_SPEED: steps = 1; break;
            case SpeedState::FULL_SPEED: steps = 2; break;
        }
        
        for (int i = 0; i < steps; i++) {
            Position moveVec = directionToVector(direction);
            Position newPos = Position(
                getPosition().getX() + moveVec.getX(),
                getPosition().getY() + moveVec.getY()
            );
            
            //check before moving if new position of the car is in bounds
            if (!world->isInside(newPos)) {
                cout << "Out of bounds! Stopping." << endl;
                speedState = SpeedState::STOPPED;
                return;
            }
            
            setPosition(newPos);
        }
    }
    
    //check if we reached gps target
    void checkTargetReached() {
        if (currentGpsTargetIndex >= gpsTargets.size()) return;
        
        Position currentPos = getPosition();
        Position target = gpsTargets[currentGpsTargetIndex];
        
        if (currentPos.manhDistance(target) <= 2) { //we reach the target in a 2 distance radius
            cout << "Reached target " << (currentGpsTargetIndex + 1) << " at " << target.toString() << endl;
            
            currentGpsTargetIndex++; //go to the next target
            
            //set next gps target
            if (currentGpsTargetIndex < gpsTargets.size()) {
                navigationSystem->setCurrentTarget(gpsTargets[currentGpsTargetIndex]);
                cout << "Next target: "<< gpsTargets[currentGpsTargetIndex].toString() << endl;
            } else {
                cout << "All targets reached!" << endl; //if all targets are reached stop
            }
        }
    }
    
};

#endif