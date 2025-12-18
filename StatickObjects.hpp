#ifndef STATICKOBJECTS_HPP
#define STATICKOBJECTS_HPP

#include <MovStatObject.hpp>

//class parked vehicle
class StationaryVehicle : public StaticObject{
    public:
        //initialize the parked vehicle
        StationaryVehicle(Position pos): StaticObject("Stationary_Vehivle", 'P' , pos){}
        ~Stationary_Vehivle(){}
        void update(int tick) {
            //it doesnt move
        }

};

class StreetSign : public StaticObject{
    protected:
        std::string text;
    
    public:
        StreetSign(Position pos, std::string t, char g): StaticObject("Street_Sign",g,pos), text(t) {}

        void update(int tick) {}

        std::string getText() const {return text;}
};


class TrafficLight : public StaticObject {
    private:
        int state; //0 for red, 1 for green, 2 for yellow
        int counter;

    public:
        TrafficLight(Position pos) : StaticObject("Traffic_Light" , 'R' , pos), state(0), counter(0) {}
        ~Traffic_Light() {}

        //update traffic light's state function
        void update(int tick) {
            counter++;
            if (state == 0 && counter >=4) { //red lasts 4 ticks and becomes green
                state =1;
                counter = 0;
                setGlyph('G');

            } else if (state==1 && counter>=8) { //green lasts 8 ticks and becomes yellow
                state = 2;
                counter = 0;
                setGlyph('Y');
            } else if (state == 2 && counter >=2) { //yellow lasts 2 ticks and becomes red
                state = 0;
                counter = 0;
                setGlyph('R');
            }
        }

        //helper functions
        bool isRed() {return state==0;}
        bool isGreen() {return state==1;}
        bool isYellow() {return state==2;}

        std::string getColor() {
            if (isRed()) {
                return "Red";
            } else if (isGreen()) {
                return "Green";
            } else {return "Yellow";}

        }
    
};

#endif