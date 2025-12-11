#ifndef REGISTRY_H
#define REGISTRY_H
#include<iostream>
#include"Flight.h"
#include"Airspace.h"
#include<fstream>
using namespace std;

class Registry {
private:
    fstream storefile;
    Flight* planes;
    int tablesize;

public:
    Registry();
    void RegisterFlight(Flight* f);
    void searchFlight(string flightId);
    void loadFlightsFromFile();
    void loadFlightInAirspace(Airspace& a);
};
#endif 
