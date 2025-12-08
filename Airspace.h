// Airspace.h
#ifndef AIRSPACE_H
#define AIRSPACE_H

#include <string>
#include "Flight.h"

using namespace std;

const int MAX_NODES    = 50;//node matlb airport ya waypoint
const int MAX_PLANES   = 50;
const int GRID_SIZE    = 20;
const int DIJKSTRA_INF = 1000000000;// very large value used to represent INFINITY

struct EdgeNode {// this is edge connected to a node(airport or waypoint)
    int to;
    int weight;
    EdgeNode* next;

    EdgeNode(int t, int w);
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

    Flight* planes[MAX_PLANES];
    int planeCount;

    char radar[GRID_SIZE][GRID_SIZE];

    int findNodeIndexByName(const string& name) const;
    int findPlaneIndexById(const string& flightId) const;

    void clearRadar();
    void drawAirportsOnRadar();
    void drawPlanesOnRadar();

public:
    Airspace();

    // graph operations
    int addAirport(const string& name, int gridX, int gridY);
    int addWaypoint(const string& name, int gridX, int gridY);
    void addEdge(const string& from, const string& to, int weight);

    // plane operations
    bool addPlaneToAirspace(Flight* flight, const string& startNodeName);
    bool movePlaneToNode(const string& flightId, const string& nodeName);
    bool removePlaneFromAirspace(const string& flightId);

    int getNodeIndexOfPlane(const string& flightId) const;

    // radar
    void updateRadar();
    void printRadar() const;

    // Dijkstra
    int findNearestAirport(int startNodeIndex,
                           int path[],
                           int& pathLength) const;

    void printGraph() const;
};

#endif
