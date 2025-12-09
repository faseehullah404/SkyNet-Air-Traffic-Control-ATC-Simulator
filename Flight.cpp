#include "Flight.h"

// Default constructor
Flight::Flight()
    : flightId(""),
    priority(MEDIUM),
    fuelPercent(100),
    origin(""),
    destination(""),
    currentNodeIndex(-1), next(nullptr)
{
}

// Parameterized constructor
Flight::Flight(const string& id,
    FlightPriority pr,
    int fuel,
    const string& from,
    const string& to, string s)
    : flightId(id),
    priority(pr),
    origin(from),
    destination(to),
    currentNodeIndex(-1),
    next(nullptr),
    status(s)
{
    // fuel safety clamp (0 to 100)
   
    if (fuel < 0) fuelPercent = 0;
    else if (fuel > 100) fuelPercent = 100;
    else fuelPercent = fuel;
}

// Determine priority using SkyNet project rules
FlightPriority calculatePriorityFromStatus(bool hasEmergency,
    int fuelPercent,
    bool isCargo)
{
    if (hasEmergency) {
        return CRITICAL;
    }
    if (fuelPercent < 10) {
        return HIGH;
    }
    if (isCargo) {
        return LOW;
    }
    return MEDIUM;
}
