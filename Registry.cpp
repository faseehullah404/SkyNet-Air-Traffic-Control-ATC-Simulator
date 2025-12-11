#include "Registry.h"
#include<iostream>
#include<sstream>   
using namespace std;

Registry::Registry() {
    tablesize = 1000;
    planes = new Flight[tablesize];

    for (int i = 0; i < tablesize; i++) {
        planes[i].next = nullptr;
    }

   
}

void Registry::loadFlightInAirspace(Airspace& a) {
    ifstream fin("flights.txt");

    if (!fin.is_open()) {
        cout << "No previous flights to load.\n";
        return;
    }

    string id, origin, dest, status, startNode;
    int index, fuel, priority;
    int startN;
    while (fin >> id >> origin >> dest >> fuel >> priority >> status >> startNode >> index) {
        startN = a.findNodeIndexByName(startNode);
        if (startN == -1) {
            cout << "Start Node Not Found!\n";
        }

        // Create new flight object from file
        Flight* f = new Flight();
        f->flightId = id;
        f->origin = origin;
        f->destination = dest;
        f->fuelPercent = fuel;
        f->priority = (FlightPriority)priority;
        f->status = status;
        f->currentNodeIndex = startN;
        f->next = nullptr;

        if (planes[index].next == nullptr)
            planes[index].next = f;
        else {
            Flight* temp = planes[index].next;
            f->next = temp;
            planes[index].next = f;
        }

        
        if (!a.isFlightExist(id)) {
            a.addPlaneToAirspace(f, startNode, true);
            
        }


    }

    fin.close();
}


void Registry::loadFlightsFromFile() {
    fstream fin;
    fin.open("flights.txt", ios::in);
    if (!fin) {
        cout << "No previous flight data found.\n";
        return;
    }

    string line;

    while (getline(fin, line)) {
        if (line == "") continue;

        stringstream ss(line);

        int index;
        string id, origin, dest, status;
        int fuel, priority;
        string startNode;

        ss >> id >> origin >> dest >> fuel >> priority >> status >> startNode >> index;
        
        // Create new flight dynamically
        Flight* f = new Flight();
        f->flightId = id;
        f->origin = origin;
        f->destination = dest;
        f->fuelPercent = fuel;
        f->priority = (FlightPriority)priority;
        f->status = status;
        f->next = nullptr;

        // Insert into hash table
        if (planes[index].next == nullptr) {
            planes[index].next = f;
        }
        else {
            f->next = planes[index].next;
            planes[index].next = f;
        }
    }

    fin.close();
}

void Registry::RegisterFlight(Flight* f) {
    int ascii = 0;
    for (int i = 0; i < f->flightId.size(); i++) {
        ascii += f->flightId[i];
    }

    ascii = ascii % tablesize;
    if (planes[ascii].next == nullptr) {
        planes[ascii].next = f;
    }
    else {
        f->next = planes[ascii].next;
        planes[ascii].next = f;
    }
    cout << ascii<<" ";
    storefile.open("flights.txt", ios::app);
    storefile << ascii << endl;
    storefile.close();
}

void Registry::searchFlight(string flightid) {
    int index = 0;
    for (int i = 0; i < flightid.size(); i++) {
        index += flightid[i];
    }

    index = index % tablesize;

    if (planes[index].next == nullptr) {
        cout << "Flight Not Found!\n";
        return;
    }
    else {
        Flight* temp = planes[index].next;
        while (temp) {
            if (temp->flightId == flightid) {
                cout << "Flight Found!\n";
                cout << "ID: " << temp->flightId << endl;
                cout << "Origin: " << temp->origin << endl;
                cout << "Destination: " << temp->destination << endl;
                cout << "Fuel: " << temp->fuelPercent << "%\n";
                cout << "Priority: " << temp->priority << endl;
                cout << "Status: " << temp->status << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Flight Not Found!\n";
    }
}
