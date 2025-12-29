/*StaticObjects.hpp*/
#ifndef STATICKOBJECTS_HPP
#define STATICKOBJECTS_HPP

#include "MovStatObject.hpp"

//class parked vehicle
class StationaryVehicle : public StaticObject{
    public:
        //initialize the parked vehicle
        StationaryVehicle(Position pos): StaticObject("PARKED", 'P', pos){}
        ~StationaryVehicle(){}  // ΔΙΟΡΘΩΣΗ: ~StationaryVehicle() όχι ~StationaryVehivle
        void update(int tick) override {
            (void)tick;
            //it doesnt move
        }
};

//class StreetSign
class StreetSign : public StaticObject{
    protected:
        std::string text; //text of the sign
    
    public:
        //constructor
        StreetSign(Position pos, std::string t, char g): StaticObject("SIGN", g, pos), text(t) {}
        //detructor
        ~StreetSign(){}
        void update(int tick) override {(void)tick;} //update function it doesnt move

        //getter
        std::string getText() const {return text;}
};

//traffic light class
class TrafficLight : public StaticObject {
    private:
        int state; //0 for red, 1 for green, 2 for yellow
        int counter;

    public:
        TrafficLight(Position pos) : StaticObject("LIGHT", 'R', pos), state(0), counter(0) {} //constructor
        ~TrafficLight() {} //destructor

        //update traffic light's state function
        void update(int tick) override {
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
            (void)tick;
        }

        //helper functions
        bool isRed() const {return state==0;}
        bool isGreen() const {return state==1;}
        bool isYellow() const {return state==2;}

        //getter
        std::string getColor() const {
            if (isRed()) {
                return "Red";
            } else if (isGreen()) {
                return "Green";
            } else {return "Yellow";}
        }
};

#endif