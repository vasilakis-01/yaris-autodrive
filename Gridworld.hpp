#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

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
    int startingX;
    int startingY;
    int destX;
    int destY;
};

class Gridworld {
private:
    int width;
    int height;
    int currentTick;

public:
    Gridworld(int w, int h)
        : width(w), height(h), currentTick(0) {
        cout << "[+WORLD] Initialized " << width << "x" << height << endl;
    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    bool isInside(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    void update() {
        currentTick++;
        cout << "[WORLD] Tick: " << currentTick << endl;
    }
};

void printHelp() {
    cout << "Self-Driving Car Simulation" << endl;
    cout << "Usage: " << endl;
    cout << "  --seed <n>                 Random seed (default: current time)" << endl;
    cout << "  --dimX <n>                 World width (default: 40)" << endl;
    cout << "  --dimY <n>                 World height (default: 40)" << endl;
    cout << "  --numMovingCars <n>        Number of moving cars (default: 3)" << endl;
    cout << "  --numMovingBikes <n>       Number of moving bikes (default: 4)" << endl;
    cout << "  --numParkedCars <n>        Number of parked cars (default: 5)" << endl;
    cout << "  --numStopSigns <n>         Number of stop signs (default: 2)" << endl;
    cout << "  --numTrafficLights <n>     Number of traffic lights (default: 2)" << endl;
    cout << "  --simulationTicks <n>      Maximum simulation ticks (default: 100)" << endl;
    cout << "  --minConfidenceThreshold <n> Minimum confidence cutoff (default: 40)" << endl;
    cout << "  --gps <x1> <y1> <x2> <y2>  GPS target coordinates (required)" << endl;
    cout << "  --help                     Show this help message" << endl << endl;
    cout << "Example usage: " << endl;
    cout << "  ./oopproj_2025 --seed 12 --dimY 50 --gps 10 20 32 15" << endl;
}

void parseArguments(int argc, char* argv[], defaults& d) {
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "--help") {
            printHelp();
            exit(0);
        }
        else if (arg == "--seed") {
            d.seed = stoul(argv[++i]);
        }
        else if (arg == "--dimX") {
            d.dimX = stoi(argv[++i]);
        }
        else if (arg == "--dimY") {
            d.dimY = stoi(argv[++i]);
        }
        else if (arg == "--numMovingCars") {
            d.numMovingCars = stoi(argv[++i]);
        }
        else if (arg == "--numMovingBikes") {
            d.numMovingBikes = stoi(argv[++i]);
        }
        else if (arg == "--numParkedCars") {
            d.numParkedCars = stoi(argv[++i]);
        }
        else if (arg == "--numStopSigns") {
            d.numStopSigns = stoi(argv[++i]);
        }
        else if (arg == "--numTrafficLights") {
            d.numTrafficLights = stoi(argv[++i]);
        }
        else if (arg == "--simulationTicks") {
            d.simulationTicks = stoi(argv[++i]);
        }
        else if (arg == "--minConfidenceThreshold") {
            d.minConfidenceThreshold = stoi(argv[++i]);
        }
        else if (arg == "--gps") {
            d.startingX = stoi(argv[++i]);
            d.startingY = stoi(argv[++i]);
            d.destX     = stoi(argv[++i]);
            d.destY     = stoi(argv[++i]);
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    defaults config;
    parseArguments(argc, argv, config);

    srand(config.seed);

    Gridworld world(config.dimX, config.dimY);

    for (int i = 0; i < config.simulationTicks; i++) {
        world.update();
    }

    return 0;
}