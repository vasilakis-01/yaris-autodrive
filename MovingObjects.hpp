#ifndef MOVINGOBJECTS_HPP
#define MOVINGOBJECTS_HPP

#include <MovStatObject.hpp>

class MovingCar : public MovingObject{
    public:
        MovingCar(Position pos,Random &range) : MovingObject("Moving_Car",'C',pos,range,1){}
        ~Moving_Car(){}
};

class MovingBike : public MovingObject{
    public:
        MovingBike(Position pos, Random &range) : MovingObject("Moving_Bike",'B',pos,range,1){}
        ~MovingBike(){}
};





#endif