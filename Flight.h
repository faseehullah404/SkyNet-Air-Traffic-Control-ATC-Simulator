
#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
using namespace std;
// enum ka matlb keh hum critical jahan bhi likhain gy to wo 1 ka barabar hoga
// high 2 ka barabar hoga
enum FlightPriority {
    CRITICAL = 1,  // Emergency, engine failure, hijack
    HIGH     = 2,  // Low fuel
    MEDIUM   = 3,  // Normal flight
    LOW      = 4   // Cargo / private plane
};

struct Flight {
    string         flightId;
    FlightPriority priority;
    int            fuelPercent;    
    string         origin;
    string         destination;

    int            currentNodeIndex;

    Flight();
    Flight(const string& id,
           FlightPriority pr,
           int fuel,
           const string& from,
           const string& to);
};


FlightPriority calculatePriorityFromStatus(bool hasEmergency,int fuelPercent,bool isCargo);

#endif
