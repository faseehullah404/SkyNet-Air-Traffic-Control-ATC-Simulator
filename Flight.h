#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
using std::string;

// Priority levels required by project
// Lower number = Higher urgency
enum FlightPriority {
    CRITICAL = 1,  // Emergency, engine failure, hijack
    HIGH = 2,  // Low fuel (<10%)
    MEDIUM = 3,  // Normal scheduled passenger flight
    LOW = 4   // Cargo / private plane
};

struct Flight {
    string flightId;
    FlightPriority priority;
    int fuelPercent;          // 0–100%
    string origin;
    string destination;
    Flight* next;
    string status;
    // Airspace node index where plane is currently located
    int currentNodeIndex;

    Flight();
    Flight(const string& id,
        FlightPriority pr,
        int fuel,
        const string& from,
        const string& to, string s);

};

// Automatically calculate priority using rules:
// Emergency > Low Fuel > Normal > Cargo
FlightPriority calculatePriorityFromStatus(bool hasEmergency,
    int fuelPercent,
    bool isCargo);

#endif
