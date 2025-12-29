/*mainprog*/
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Gridworld.hpp"
#include "StaticObjects.hpp"
#include "MovingObjects.hpp"
#include "SelfDrivingCar.hpp"

using namespace std;

//struct with defaults for objects and seed if there are not given as arguements
struct defaults {
    unsigned int seed = time(nullptr);
    int dimX = 40;
    int dimY = 40;
    int numMovingCars = 3;
    int numMovingBikes = 4;
    int numParkedCars = 5;
    int numStopSigns = 2;
    int numTrafficLights = 2;
    int simulationTicks = 100;
    int minConfidenceThreshold = 40;
    int startX = -1;
    int startY = -1;
    int destX = -1;
    int destY = -1;
    
    bool hasGPS() const {
        return startX != -1 && startY != -1 && destX != -1 && destY != -1;
    }
};

//help function to give correct arguements
void printHelp() {
    cout << "Self-Driving Car Simulation\nUsage:\n";
    cout << "  --seed <n>                 Random seed (default: current time)\n";
    cout << "  --dimX <n>                 World width (default: 40)\n";
    cout << "  --dimY <n>                 World height (default: 40)\n";
    cout << "  --numMovingCars <n>        Number of moving cars (default: 3)\n";
    cout << "  --numMovingBikes <n>       Number of moving bikes (default: 4)\n";
    cout << "  --numParkedCars <n>        Number of parked cars (default: 5)\n";
    cout << "  --numStopSigns <n>         Number of stop signs (default: 2)\n";
    cout << "  --numTrafficLights <n>     Number of traffic lights (default: 2)\n";
    cout << "  --simulationTicks <n>      Maximum simulation ticks (default: 100)\n";
    cout << "  --viewRadius <n>           POV view radius (default: 3)\n";
    cout << "  --frontView                Use front view instead of centered\n";
    cout << "  --gps <x1> <y1> <x2> <y2>  GPS target coordinates (required)\n";
    cout << "  --help                     Show this help message\n\n";
    cout << "Example: ./oopproj --seed 12 --viewRadius 5 --gps 10 20 32 15\n";
}

//function to parse arguements and do the nexessary input checks
void parseArguments(int argc, char* argv[], defaults& d, int& viewRadius, bool& frontView) {
    viewRadius = 3;
    frontView = false;
    
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--help") { printHelp(); exit(0); }
        else if (arg == "--seed" && i+1 < argc) d.seed = stoul(argv[++i]);
        else if (arg == "--dimX" && i+1 < argc) d.dimX = stoi(argv[++i]);
        else if (arg == "--dimY" && i+1 < argc) d.dimY = stoi(argv[++i]);
        else if (arg == "--numMovingCars" && i+1 < argc) d.numMovingCars = stoi(argv[++i]);
        else if (arg == "--numMovingBikes" && i+1 < argc) d.numMovingBikes = stoi(argv[++i]);
        else if (arg == "--numParkedCars" && i+1 < argc) d.numParkedCars = stoi(argv[++i]);
        else if (arg == "--numStopSigns" && i+1 < argc) d.numStopSigns = stoi(argv[++i]);
        else if (arg == "--numTrafficLights" && i+1 < argc) d.numTrafficLights = stoi(argv[++i]);
        else if (arg == "--simulationTicks" && i+1 < argc) d.simulationTicks = stoi(argv[++i]);
        else if (arg == "--viewRadius" && i+1 < argc) viewRadius = stoi(argv[++i]);
        else if (arg == "--frontView") frontView = true;
        else if (arg == "--gps" && i+4 < argc) {
            d.startX = stoi(argv[++i]);
            d.startY = stoi(argv[++i]);
            d.destX = stoi(argv[++i]);
            d.destY = stoi(argv[++i]);
        }
    }
}

int main(int argc, char* argv[]) {
    defaults config;
    int viewRadius;
    bool frontView;
    
    parseArguments(argc, argv, config, viewRadius, frontView);
    
    if (!config.hasGPS()) {
        cout << "Error: GPS coordinates required! Use --gps x1 y1 x2 y2\n";
        printHelp();
        return 1;
    }
    //prints
    cout << "=== Self Driving Car Simulation ===\n";
    cout << "World: " << config.dimX << "x" << config.dimY 
         << " | Seed: " << config.seed << "\n";
    cout << "GPS: (" << config.startX << "," << config.startY 
         << ") -> (" << config.destX << "," << config.destY << ")\n";
    cout << "Ticks: " << config.simulationTicks 
         << " | View: " << (frontView ? "Front" : "Centered") 
         << " (radius: " << viewRadius << ")\n";
    cout << "==================================\n";
    
    //world creation
    GridWorld world(config.dimX, config.dimY);
    Random rng(config.seed);
    vector<Position> occupied;
    
    //object creations
    cout << "\nCreating objects...\n";
    
    //parked cars
    for (int i = 0; i < config.numParkedCars; i++) {
        Position pos = getRandomPosition(rng, config.dimX, config.dimY, occupied);
        StationaryVehicle* parkedCar = new StationaryVehicle(pos);
        if (world.addObject(parkedCar)) {
            occupied.push_back(pos);
            cout << "[PARKED] at " << pos.toString() << endl;
        } else {
            delete parkedCar;  //cleanup if needed
        }
    }
    
    //stop signs
    for (int i = 0; i < config.numStopSigns; i++) {
        Position pos = getRandomPosition(rng, config.dimX, config.dimY, occupied);
        StreetSign* stopSign = new StreetSign(pos, "STOP", 'S');
        if (world.addObject(stopSign)) {
            occupied.push_back(pos);
            cout << "[STOP SIGN] at " << pos.toString() << endl;
        } else {
            delete stopSign;
        }
    }
    
    //traffic lights
    for (int i = 0; i < config.numTrafficLights; i++) {
        Position pos = getRandomPosition(rng, config.dimX, config.dimY, occupied);
        TrafficLight* light = new TrafficLight(pos);
        if (world.addObject(light)) {
            occupied.push_back(pos);
            cout << "[TRAFFIC LIGHT] at " << pos.toString() << endl;
        } else {
            delete light;
        }
    }
    
    //moving cars
    for (int i = 0; i < config.numMovingCars; i++) {
        Position pos = getRandomPosition(rng, config.dimX, config.dimY, occupied);
        MovingCar* car = new MovingCar(pos, rng);
        if (world.addObject(car)) {
            occupied.push_back(pos);
            cout << "[MOVING CAR] at " << pos.toString() << endl;
        } else {
            delete car;
        }
    }
    
    //moving bikes
    for (int i = 0; i < config.numMovingBikes; i++) {
        Position pos = getRandomPosition(rng, config.dimX, config.dimY, occupied);
        MovingBike* bike = new MovingBike(pos, rng);
        if (world.addObject(bike)) {
            occupied.push_back(pos);
            cout << "[MOVING BIKE] at " << pos.toString() << endl;
        } else {
            delete bike;
        }
    }
    
    //FULL VISUALIZATION
    cout << "\n=== INITIAL STATE (Full Visualization) ===\n";
    world.visualization_full();
    
    //create self driving car
    cout << "\n=== CREATING AUTONOMOUS CAR ===\n";
    Position startPos(config.startX, config.startY);
    Position targetPos(config.destX, config.destY);
    
    //check if the starting posisiton of the car is availiable
    if (!world.isInside(startPos)) {
        cout << "Error: Starting position " << startPos.toString() 
             << " is outside world bounds!\n";
        return 1;
    }
    
    //if occupied
    if (world.getObjectAt(startPos) != nullptr) {
        cout << "Warning: Starting position occupied! Finding alternative...\n";
        //find new postion close to the one given
        bool found = false;
        for (int radius = 1; radius <= 5 && !found; radius++) {
            for (int dx = -radius; dx <= radius && !found; dx++) {
                for (int dy = -radius; dy <= radius && !found; dy++) {
                    Position altPos(startPos.getX() + dx, startPos.getY() + dy);
                    if (world.isInside(altPos) && world.getObjectAt(altPos) == nullptr) {
                        startPos = altPos;
                        found = true;
                        cout << "New starting position: " << startPos.toString() << endl;
                    }
                }
            }
        }
        if (!found) {
            cout << "Error: Could not find empty starting position!\n";
            return 1;
        }
    }
    
    //self driving car
    SelfDrivingCar* autonomousCar = new SelfDrivingCar(startPos, Direction::EAST, &world);
    
    //set gps targets
    vector<Position> gpsTargets = {targetPos};
    autonomousCar->setGPSTargets(gpsTargets);
    
    //add the self driving car to the world
    if (!world.addObject(autonomousCar)) {
        cout << "Error: Could not add autonomous car to world!\n";
        delete autonomousCar;  //ceanup
        return 1;
    }
    
    cout << "\n=== STARTING SIMULATION ===\n";
    cout << "Autonomous car ID: " << autonomousCar->getId() << endl;
    cout << "Starting at: " << startPos.toString() << endl;
    cout << "Target: " << targetPos.toString() << endl;
    
    //excecute simulation
    bool targetReached = false;
    for (int tick = 0; tick < config.simulationTicks; tick++) {
        cout << "\n--- Tick " << tick << " ---\n";
        
        //update all objects
        world.update(tick);
        
        //update self driving car
        autonomousCar->update(tick);
        
        //pov visualization
        world.visualization_pov(autonomousCar->getPosition(), viewRadius, !frontView);
        
        //check if simulation was successfull
        if (autonomousCar->getCurrentTargetIndex() >= autonomousCar->getTotalTargets()) {
            cout << "\n✓ ALL TARGETS REACHED! ✓\n";
            targetReached = true;
            break;
        }
    }
    
    //FULL VISUALIZATION at the end
    cout << "\n=== FINAL STATE (Full Visualization) ===\n";
    world.visualization_full();
    
    cout << "\n=== SIMULATION COMPLETE ===\n";
    cout << "Final position: " << autonomousCar->getPosition().toString() << "\n";
    cout << "Target: (" << config.destX << "," << config.destY << ")\n";
    cout << "Distance: " << autonomousCar->getPosition().manhDistance(targetPos) << " cells\n";
    
    if (!targetReached) {
        cout << "Simulation stopped after " << config.simulationTicks << " ticks\n";
    }
    
    //cleanup
    Position carPos = autonomousCar->getPosition();
    if (world.getObjectAt(carPos) == autonomousCar) {
        world.clearCarPointer(autonomousCar);
    }
    
    return 0;
}