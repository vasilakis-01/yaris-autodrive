#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace std;

#include "MovingsObjects.hpp"
#include "Gridworld.hpp"

enum class SpeedState {
    STOPPED = 0,
    HALF_SPEED = 1,
    FULL_SPEED = 2
};

class GridWorld;

class SelfDrivingCar : public MovingObject {
private:
    SpeedState speedState;
    int maxSpeed;

    vector<unique_ptr<Sensor>> sensors;
    unique_ptr<NavigationSystem> navigationSystem;

public:
    SelfDrivingCar(
        const string& id,
        const Position& startPos,
        Direction dir,
        GridWorld* world
    );

    ~SelfDrivingCar();

    void update() override;

    vector<SensorReading> collectSensorData();
    void syncNavigationSystem(const vector<SensorReading>& readings);

    void executeMovement();
    void accelerate();
    void decelerate();
    void turn(Direction newDirection);

    SpeedState getSpeedState() const
    int getCurrentSpeed() const;

    void addSensor(unique_ptr<Sensor> sensor);
};

SelfDrivingCar::SelfDrivingCar(
    const string& id,
    const Position& startPos,
    Direction dir,
    GridWorld* world
)
    : MovingObject(id, startPos, dir, world),
      speedState(SpeedState::STOPPED),
      maxSpeed(2)
{
    glyph = '@';

    navigationSystem = make_unique<NavigationSystem>(id + "_NAV");

    cout << "[+VEHICLE: " << id << "] Created at ("
         << startPos.x << "," << startPos.y
         << "), heading " << directionToString(dir)
         << " at STOPPED speed" << endl;
}

SelfDrivingCar::~SelfDrivingCar() {
    cout << "[-CAR: " << id << "] Our journey is complete!" << endl;
}

void SelfDrivingCar::update() {
    auto readings = collectSensorData();
    syncNavigationSystem(readings);
    executeMovement();
}

vector<SensorReading> SelfDrivingCar::collectSensorData() {
    vector<SensorReading> allReadings;

    for (const auto& sensor : sensors) {
        auto sensorReadings = sensor->sense(position, direction);
        allReadings.insert(
            allReadings.end(),
            sensorReadings.begin(),
            sensorReadings.end()
        );
    }

    return allReadings;
}

void SelfDrivingCar::syncNavigationSystem(
    const vector<SensorReading>& readings
) {
    navigationSystem->updateSensorData(readings);
}

void SelfDrivingCar::executeMovement() {
    NavigationDecision decision =
        navigationSystem->makeDecision(position, direction);

    if (decision.decelerate)
        decelerate();
    else if (decision.accelerate)
        accelerate();

    if (decision.turn && decision.newDirection != direction)
        turn(decision.newDirection);

    int steps = getCurrentSpeed();
    for (int i = 0; i < steps; ++i) {
        Position nextPos = position.next(direction);

        if (!world->isInside(nextPos)) {
            world->removeObject(this);
            return;
        }

        position = nextPos;
    }
}

void SelfDrivingCar::accelerate() {
    if (speedState == SpeedState::STOPPED)
        speedState = SpeedState::HALF_SPEED;
    else if (speedState == SpeedState::HALF_SPEED)
        speedState = SpeedState::FULL_SPEED;
}

void SelfDrivingCar::decelerate() {
    if (speedState == SpeedState::FULL_SPEED)
        speedState = SpeedState::HALF_SPEED;
    else if (speedState == SpeedState::HALF_SPEED)
        speedState = SpeedState::STOPPED;
}

void SelfDrivingCar::turn(Direction newDirection) {
    direction = newDirection;
}

SpeedState SelfDrivingCar::getSpeedState() const {
    return speedState;
}

int SelfDrivingCar::getCurrentSpeed() const {
    switch (speedState) {
        case SpeedState::STOPPED: return 0;
        case SpeedState::HALF_SPEED: return 1;
        case SpeedState::FULL_SPEED: return maxSpeed;
    }
    return 0;
}

void SelfDrivingCar::addSensor(unique_ptr<Sensor> sensor) {
    sensors.push_back(move(sensor));
}
