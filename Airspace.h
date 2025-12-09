#ifndef AIRSPACE_H
#define AIRSPACE_H

#include <string>
#include "Flight.h"
#include<fstream>
using namespace std;

const int MAX_NODES = 50;      // node = airport or waypoint
const int MAX_PLANES = 50;
const int GRID_SIZE = 20;
const int DIJKSTRA_INF = 1000000000; // very large value used to represent INFINITY

struct EdgeNode {
    int to;
    int weight;
    EdgeNode* next;
    string name;
    EdgeNode(int t, int w, string n);
};

struct GraphNode {
    string name;
    int gridX;
    int gridY;
    bool isAirport;
    EdgeNode* head;

    GraphNode();
};

class Airspace {
private:
    GraphNode nodes[MAX_NODES];
    int nodeCount;
    fstream airport;

    Flight* planes[MAX_PLANES];
    int planeCount;

    char radar[GRID_SIZE][GRID_SIZE];

    int findPlaneIndexById(const string& flightId) const;
    bool isNodeOccupied(int nodeIndex) const;

    void clearRadar();
    void drawAirportsOnRadar();
    void drawPlanesOnRadar();

    // free memory for edges
    void freeEdges();

public:
    Airspace();
    ~Airspace();
    int findNodeIndexByName(const string& name) const;
    // non-copyable (simple prevention)
    Airspace(const Airspace&) = delete;
    Airspace& operator=(const Airspace&) = delete;
    bool isAirportAtIndex(int i);
    // graph operations
    int addAirport(const string& name, int gridX, int gridY, bool isfile = false);
    int addWaypoint(const string& name, int gridX, int gridY);
    bool addEdge(const string& from, const string& to, int weight);

    bool isFlightExist(string);
    // plane operations
    bool addPlaneToAirspace(Flight* flight, const string& startNodeName, bool ff = false);
    bool movePlaneToNode(const string& flightId, const string& nodeName);
    bool removePlaneFromAirspace(const string& flightId);

    int getNodeIndexOfPlane(const string& flightId) const;

    string getNameByIndex(int i);
    // radar
    void updateRadar();
    void printRadar() const;

    // Dijkstra
    // path must be an array of size >= MAX_NODES
    // returns nearest airport index or -1 if none. pathLength will be set.
    int findNearestAirport(int startNodeIndex, int path[], int& pathLength) const;

    void printGraph() const;
    void printSafeRoute(const string& flightId);

};

#endif // AIRSPACE_H
