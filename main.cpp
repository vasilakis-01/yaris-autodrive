#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Gridworld.hpp"
#include "StaticObjects.hpp"
#include "MovingObjects.hpp"

using namespace std;

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
    cout << "  --gps <x1> <y1> <x2> <y2>  GPS target coordinates (required)\n";
    cout << "  --help                     Show this help message\n\n";
    cout << "Example: ./oopproj --seed 12 --gps 10 20 32 15\n";
}

void parseArguments(int argc, char* argv[], defaults& d) {
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
    parseArguments(argc, argv, config);
    
    if (!config.hasGPS()) {
        cout << "Error: GPS coordinates required! Use --gps x1 y1 x2 y2\n";
        printHelp();
        return 1;
    }
    
    cout << "=== Self-Driving Car Simulation ===\n";
    cout << "World: " << config.dimX << "x" << config.dimY 
         << " | Seed: " << config.seed << "\n";
    cout << "GPS: (" << config.startX << "," << config.startY 
         << ") -> (" << config.destX << "," << config.destY << ")\n";
    cout << "Ticks: " << config.simulationTicks << "\n";
    
    GridWorld world(config.dimX, config.dimY);
    Random rng(config.seed);
    vector<Position> occupied;
    
    // Δημιουργία αντικειμένων
    cout << "\nCreating objects...\n";
    
    // Parked cars
    for (int i = 0; i < config.numParkedCars; i++) {
        Position pos = getRandomPosition(rng, config.dimX, config.dimY, occupied);
        if (world.addObject(new StationaryVehicle(pos))) {
            occupied.push_back(pos);
        }
    }
    
    // Stop signs
    for (int i = 0; i < config.numStopSigns; i++) {
        Position pos = getRandomPosition(rng, config.dimX, config.dimY, occupied);
        if (world.addObject(new StreetSign(pos, "STOP", 'S'))) {
            occupied.push_back(pos);
        }
    }
    
    // Traffic lights
    for (int i = 0; i < config.numTrafficLights; i++) {
        Position pos = getRandomPosition(rng, config.dimX, config.dimY, occupied);
        if (world.addObject(new TrafficLight(pos))) {
            occupied.push_back(pos);
        }
    }
    
    // Moving cars
    for (int i = 0; i < config.numMovingCars; i++) {
        Position pos = getRandomPosition(rng, config.dimX, config.dimY, occupied);
        if (world.addObject(new MovingCar(pos, rng))) {
            occupied.push_back(pos);
        }
    }
    
    // Moving bikes
    for (int i = 0; i < config.numMovingBikes; i++) {
        Position pos = getRandomPosition(rng, config.dimX, config.dimY, occupied);
        if (world.addObject(new MovingBike(pos, rng))) {
            occupied.push_back(pos);
        }
    }
    
    cout << "\n=== Initial State ===\n";
    world.show();
    
    cout << "\n=== Running Simulation ===\n";
    for (int tick = 0; tick < config.simulationTicks; tick++) {
        world.update(tick);
        if (tick == 0 || (tick + 1) % 20 == 0) {
            cout << "Tick " << (tick + 1) << ":\n";
            world.show();
        }
    }
    
    cout << "\n=== Final State ===\n";
    world.show();
    
    cout << "\nSimulation completed!\n";
    return 0;
}