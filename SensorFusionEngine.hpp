/*SensorFusionEngine.hpp*/
#ifndef SENSORFUSIONENGINE_HPP
#define SENSORFUSIONENGINE_HPP

#include "SensorReading.hpp"
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

//Sensor fusion engine class
class SensorFusionEngine {
    private:
        double minConfidenceThreshold; //minimum confidence
    
    public:
        SensorFusionEngine(double threshold = 0.4): minConfidenceThreshold(threshold) {} //during construction min_conf = 40% 
    
        vector<SensorReading> fuseSensorData(const vector<SensorReading>& allReadings) {
            //check if data is collected
            if (allReadings.empty()) {
                cout << "[FUSION] No readings to fuse" << endl;
                return {};
            }
            
            //store object's according to their id
            map<string, vector<SensorReading>> readingsByObjectId;

            //take into consideration only meauremnts with confidence above 40% (for bikes it's above 20%)
            for (const auto& reading : allReadings) {
                if (reading.confidence < minConfidenceThreshold) {
                    if (reading.objectType == "BIKE" && reading.confidence > 0.1) {
                        readingsByObjectId[reading.objectId].push_back(reading);
                    }
                    continue;
                }
                readingsByObjectId[reading.objectId].push_back(reading);
            }
            
            //add data to fused readings vector and print the data that is being stored
            vector<SensorReading> fusedReadings;
        
            for (auto& pair : readingsByObjectId) {
                cout << "[FUSION] Processing object: " << pair.first << endl;
                vector<SensorReading>& readings = pair.second;
            
                if (readings.empty()) continue;
            
                if (readings.size() == 1) {
                    fusedReadings.push_back(readings[0]);
                } else {
                    SensorReading fused = fuseReadingsForObject(readings);
                    fusedReadings.push_back(fused);
                }
            }
        
            cout << "[FUSION] Fused " << allReadings.size() << " readings into " << fusedReadings.size() << " objects" << endl;
        
            return fusedReadings;
        }
    
        SensorReading fuseReadingsForObject(const vector<SensorReading>& readings) {
            if (readings.empty()) return SensorReading();
        
            SensorReading fused = readings[0];//begin from the first measurement
        
            //compare each sensor's measurement's confidence and store the one with the higher confidnence
            for (size_t i = 1; i < readings.size(); i++) {
                if (readings[i].confidence > fused.confidence) {
                    //keep the "better" measurmeent
                    fused = readings[i];
                } else if (readings[i].confidence == fused.confidence) {
                    //fill in the missing info
                    if (fused.signText.empty() && !readings[i].signText.empty()) {
                        fused.signText = readings[i].signText;
                    }
                    if (fused.trafficLightColor.empty() && !readings[i].trafficLightColor.empty()) {
                        fused.trafficLightColor = readings[i].trafficLightColor;
                    }
                    if (fused.speed == 0 && readings[i].speed > 0) {
                        fused.speed = readings[i].speed;
                        fused.direction = readings[i].direction;
                    }
                }
            }
        
            //increment a little the minimum confidence because more and more measurements are being done
            fused.confidence = min(1.0, fused.confidence * 1.1);
        
            return fused;
        }
        //setter
        void setMinConfidenceThreshold(double threshold) {
            minConfidenceThreshold = threshold;
        }
        //getter
        double getMinConfidenceThreshold() const {
            return minConfidenceThreshold;
        }
};

#endif