/*MovingObjects.hpp*/
#ifndef MOVINGOBJECTS_HPP
#define MOVINGOBJECTS_HPP

#include "MovStatObject.hpp"

class MovingCar : public MovingObject{
    public:
        MovingCar(Position pos, Random &rng) : MovingObject("CAR", 'C', pos, randomDirection(rng), 1){}
        ~MovingCar(){}

        void update(int tick) override {
            MovingObject::update(tick);
            (void)tick;
        }
};

class MovingBike : public MovingObject{
    public:
        MovingBike(Position pos, Random &rng) : MovingObject("BIKE", 'B', pos, randomDirection(rng), 1){}
        ~MovingBike(){}

        void update(int tick) override {
            MovingObject::update(tick);
            (void)tick;
        }
};

#endif