
#include "Flight.h"

using namespace std;

Flight::Flight(): flightId(""),priority(MEDIUM),fuelPercent(100),origin(""),destination(""),currentNodeIndex(-1) {}

Flight::Flight(const string& id,FlightPriority pr,int fuel,const string& from,const string& to)
    : flightId(id),priority(pr),fuelPercent(fuel),origin(from),destination(to),currentNodeIndex(-1) {}

FlightPriority calculatePriorityFromStatus(bool hasEmergency,int fuelPercent,bool isCargo)
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
