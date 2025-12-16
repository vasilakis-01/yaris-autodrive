#include <iostream>

using namespace std;
unsigned int seed;   //global(ebgaze themata alliws)
class Gridworld{
    private:
        int width;
        int height;
    public:
        Gridworld(int wid,int h);
        int getWidth
}

struct defaults{
    unsigned int seed;
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
void printHelp(){
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
        cout << "  --gps <x1> <y1> [x2 y2 ...] GPS target coordinates (required)" << endl;
        cout << "  --help                     Show this help message" << endl << endl;
        cout << "Example usage: " << endl;
        cout << "  ./oopproj_2025 --seed 12 --dimY 50 --gps 10 20 32 15" << endl;
    }

void parseArguments(int argc,char *argv[]){
     for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--help") {
            printHelp();
            return 0;
        }
        else if (arg == "--seed") {
            seed = stoul(argv[++i]);
        }
        else if (arg == "--dimX") {
            dimX = stoi(argv[++i]);
        }
        else if (arg == "--dimY") {
            dimY = stoi(argv[++i]);
        }
        else if (arg == "--numMovingCars"){
            numMovingCars = stoi(argv[++i]);
        }
        else if (arg == "--numMovingBikes"){
            numMovingBikes = stoi(argv[++i]);
        }
        else if (arg == "--numParkedCars"){
            numParkedCars = stoi(argv[++i]);
        }
        else if (arg == "--numStopSigns"){
            numStopSigns = stoi(argv[++i]);
        }
        else if (arg == "--numTrafficLights"){
            numTrafficLights = stoi(argv[++i]);
        }
        else if (arg == "--simulationTicks"){
            simulationTicks = stoi(argv[++i]);
        }
        else if (arg == "--minConfidenceThreshold"){
            minConfidenceThreshold = stoi(argv[++i]);
        }
        else if (arg == "--gps"){
            startingX = stoi(argv[i+1]);
            startingY = stoi(argv[i+2]);
            destX= stoi(argv[i+3]);
            destY= stoi(argv[i+4]);
        }
}
}

int main(int argc, char* argv[]){
    parseArguments(argc,argv);
    return 0;
}