#include "Airspace.h"
#include <iostream>
#include <cstring> // for memset
using namespace std;


// EdgeNode & GraphNode
EdgeNode::EdgeNode(int t, int w, string n) : to(t), weight(w), next(nullptr), name(n) {}

GraphNode::GraphNode() : name(""), gridX(0), gridY(0), isAirport(false), head(nullptr) {}

// Airspace constructor
Airspace::Airspace() : nodeCount(0), planeCount(0) {
    // initialize planes[] to nullptr
    for (int i = 0; i < MAX_PLANES; ++i) {
        planes[i] = nullptr;
    }

    ifstream fin("airports.txt");

    if (fin.is_open()) {
        string name;
        int x, y;
        bool isairport;

        while (fin >> name >> x >> y >> isairport) {
            if (isairport) {
                addAirport(name, x, y, true);
            }
            
        }

        fin.close();
    }

    clearRadar();
}


Airspace::~Airspace() {
    freeEdges();
}

// Free all EdgeNode memory
void Airspace::freeEdges() {
    for (int i = 0; i < nodeCount; ++i) {
        EdgeNode* e = nodes[i].head;
        while (e != nullptr) {
            EdgeNode* next = e->next;
            delete e;
            e = next;
        }
        nodes[i].head = nullptr;
    }
}

// Private helpers
int Airspace::findNodeIndexByName(const string& name) const {
    for (int i = 0; i < nodeCount; ++i) {
        if (nodes[i].name == name) {
            return i;
        }
    }
    return -1;
}

int Airspace::findPlaneIndexById(const string& flightId) const {
    for (int i = 0; i < planeCount; ++i) {
        if (planes[i] != nullptr && planes[i]->flightId == flightId)
            return i;
    }
    return -1;
}

bool Airspace::isNodeOccupied(int nodeIndex) const {
    if (nodeIndex < 0 || nodeIndex >= nodeCount) return false;
    for (int i = 0; i < planeCount; ++i) {
        if (planes[i] != nullptr && planes[i]->currentNodeIndex == nodeIndex) return true;
    }
    return false;
}

void Airspace::clearRadar() {
    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE; ++c) {
            radar[r][c] = '.';
        }
    }
}

void Airspace::drawAirportsOnRadar() {
    for (int i = 0; i < nodeCount; ++i) {
        if (nodes[i].isAirport) {
            int x = nodes[i].gridX;
            int y = nodes[i].gridY;
            if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) radar[y][x] = 'A';
        }
    }
}

void Airspace::drawPlanesOnRadar() {
    for (int i = 0; i < planeCount; ++i) {
        if (planes[i] != nullptr) {
            int nodeIndex = planes[i]->currentNodeIndex;
            if (nodeIndex >= 0 && nodeIndex < nodeCount) {
                int x = nodes[nodeIndex].gridX;
                int y = nodes[nodeIndex].gridY;
                if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
                    // In case of airport + plane, plane should overwrite airport marker visually
                    radar[y][x] = 'P';
                }
            }
        }
    }
}


int Airspace::addAirport(const string& name, int gridX, int gridY, bool isfile) {
    if (nodeCount >= MAX_NODES) {
        cout << "Cannot add more nodes (MAX_NODES reached).\n";
        return -1;
    }
    if (findNodeIndexByName(name) != -1) {
        return -1;
    }
    if (!isfile) {
        ofstream fout("airports.txt", ios::app);
        if (!fout.is_open()) {
            cout << "Error opening file!\n";
        }
        else {

            if (findNodeIndexByName(name) != -1) {
                return 0;
            }
            else {
                fout << name << " " << gridX << " " << gridY << " " << 1 << endl;
                fout.close();
            }
        }
    }
    int idx = nodeCount++;
    nodes[idx].name = name;
    nodes[idx].gridX = gridX;
    nodes[idx].gridY = gridY;
    nodes[idx].isAirport = true;
    nodes[idx].head = nullptr;

    // ---- FIXED FILE HANDLING ----
    

    updateRadar();
    return idx;
}


int Airspace::addWaypoint(const string& name, int gridX, int gridY) {
    if (nodeCount >= MAX_NODES) {
        cout << "Cannot add more nodes (MAX_NODES reached).\n";
        return -1;
    }
    if (findNodeIndexByName(name) != -1) {
        cout << "Node with name '" << name << "' already exists.\n";
        return -1;
    }

    int idx = nodeCount++;
    nodes[idx].name = name;
    nodes[idx].gridX = gridX;
    nodes[idx].gridY = gridY;
    nodes[idx].isAirport = false;
    nodes[idx].head = nullptr;

    updateRadar();
    return idx;
}
//Adding airports and waypoints through adjacency list.
bool Airspace::addEdge(const string& from, const string& to, int weight) {
    int fromIdx = findNodeIndexByName(from);
    int toIdx = findNodeIndexByName(to);

    if (fromIdx == -1 || toIdx == -1) {
        cout << "addEdge error: invalid node name(s).\n";
        return false;
    }

    // Prevent duplicate edge 
    EdgeNode* e = nodes[fromIdx].head;
    
    while (e != nullptr) {
        if (e->to == toIdx && e->weight == weight) {
            // same edge already exists
            return true;
        }
        e = e->next;
    }

    EdgeNode* ne = new EdgeNode(toIdx, weight, to);
    ne->next = nodes[fromIdx].head;
    nodes[fromIdx].head = ne;
    return true;
}

// Public: Planes in airspace

bool Airspace::addPlaneToAirspace(Flight* flight, const string& startNodeName, bool ff) {
    if (planeCount >= MAX_PLANES) {
        cout << "Cannot add more planes (MAX_PLANES reached).\n";
        return false;
    }
    int nodeIdx = findNodeIndexByName(startNodeName);
    if (nodeIdx == -1) {
        cout << "addPlaneToAirspace error: start node does not exist.\n";
        return false;
    }

    if (!ff) {
        flight->currentNodeIndex = nodeIdx;
        fstream file;
        file.open("flights.txt", ios::app);
        file << flight->flightId << " " << flight->origin << " " << flight->destination << " " << flight->fuelPercent << " " << flight->priority << " " << flight->status << " " << startNodeName << " ";
        planes[planeCount++] = flight;
    }
    else {
        flight->currentNodeIndex = nodeIdx;
        
        planes[planeCount++] = flight;
    }
    updateRadar();
    return true;
}

bool Airspace::isAirportAtIndex(int i) {
    if (nodes[i].isAirport) {
        return true;

    }
    else {
        return false;
    }
}

bool Airspace::isFlightExist(string id) {
    for (int i = 0; i < planeCount; i++) {
        if (planes[i]->flightId == id) {
            return true;
        }
    }
    return false;
}
bool Airspace::movePlaneToNode(const string& flightId, const string& nodeName) {
    int planeIdx = findPlaneIndexById(flightId);
    if (planeIdx == -1) {
        cout << "movePlaneToNode error: plane not found.\n";
        return false;
    }

    int nodeIdx = findNodeIndexByName(nodeName);
    if (nodeIdx == -1) {
        cout << "movePlaneToNode error: node not found.\n";
        return false;
    }

    // collision avoidance
    if (isNodeOccupied(nodeIdx)) {
        cout << "movePlaneToNode: destination node '" << nodeName << "' is occupied — holding plane " << flightId << ".\n";
        return false;
    }

    planes[planeIdx]->currentNodeIndex = nodeIdx;
    updateRadar();
    return true;
}

bool Airspace::removePlaneFromAirspace(const string& flightId) {
    int idx = findPlaneIndexById(flightId);
    if (idx == -1) {
        cout << "removePlaneFromAirspace error: plane not found.\n";
        return false;
    }

    // Shift remaining planes left
    for (int i = idx; i < planeCount - 1; ++i) {
        planes[i] = planes[i + 1];
    }
    planes[planeCount - 1] = nullptr;
    planeCount--;

    updateRadar();
    return true;
}

int Airspace::getNodeIndexOfPlane(const string& flightId) const {
    int planeIdx = findPlaneIndexById(flightId);
    if (planeIdx == -1) return -1;
    return planes[planeIdx]->currentNodeIndex;
}

// Radar handling
void Airspace::updateRadar() {
    clearRadar();
    drawAirportsOnRadar();
    drawPlanesOnRadar();
}

void Airspace::printRadar() const {
    cout << "   ";
    for (int c = 0; c < GRID_SIZE; ++c) {
        if (c < 10) {
            cout <<' ' << c;
        }
        else {
            cout << c;
        }
        if (c < GRID_SIZE - 1) {
            cout << ' ';
        }
    }
    cout << endl;

    for (int r = 0; r < GRID_SIZE; ++r) {
        if (r < 10) {
            cout << ' ' << r << ' ';
        }
        else {
            cout << r << ' ';
        }
        for (int c = 0; c < GRID_SIZE; ++c) {
            cout << radar[r][c] << "  ";
        }
        cout << endl;
    }

    cout << "Legend: A = Airport, P = Plane, . = Empty sky\n";
}

// Dijkstra: nearest airport
int Airspace::findNearestAirport(int startNodeIndex, int path[], int& pathLength) const {
    if (startNodeIndex < 0 || startNodeIndex >= nodeCount) {
        pathLength = 0;
        return -1;
    }

    int dist[MAX_NODES];
    bool visited[MAX_NODES];
    int prev[MAX_NODES];

    for (int i = 0; i < nodeCount; ++i) {
        dist[i] = DIJKSTRA_INF;
        visited[i] = false;
        prev[i] = -1;
    }

    dist[startNodeIndex] = 0;

    // Simple O(V^2) Dijkstra (no STL heap)
    for (int count = 0; count < nodeCount; ++count) {
        int u = -1;
        int bestDist = DIJKSTRA_INF;

        // Find unvisited node with smallest dist
        for (int i = 0; i < nodeCount; ++i) {
            if (!visited[i] && dist[i] < bestDist) {
                bestDist = dist[i];
                u = i;
            }
        }

        if (u == -1) break; // remaining nodes are unreachable

        visited[u] = true;

        // Relax edges
        EdgeNode* e = nodes[u].head;
        while (e != nullptr) {
            int v = e->to;
            int w = e->weight;

            if (!visited[v] && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
            }
            e = e->next;
        }
    }

    // Find nearest airport
    int nearestAirport = -1;
    int nearestAirportDist = DIJKSTRA_INF;

    for (int i = 0; i < nodeCount; ++i) {
        if (nodes[i].isAirport && dist[i] < nearestAirportDist) {
            nearestAirportDist = dist[i];
            nearestAirport = i;
        }
    }

    if (nearestAirport == -1) {
        pathLength = 0;
        return -1;
    }

    // Reconstruct path from startNodeIndex -> nearestAirport
    int tempPath[MAX_NODES];
    int tempLen = 0;

    int curr = nearestAirport;
    while (curr != -1) {
        tempPath[tempLen++] = curr;
        curr = prev[curr];
    }

    // Reverse into path[]
    pathLength = 0;
    for (int i = tempLen - 1; i >= 0; --i) {
        path[pathLength++] = tempPath[i];
    }

    return nearestAirport;
}

string Airspace::getNameByIndex(int i) {
    return nodes[i].name;
}

// Debug: print graph
void Airspace::printGraph() const {
    cout << "Graph nodes (" << nodeCount << "):\n";
    for (int i = 0; i < nodeCount; ++i) {
        cout << i << ": " << nodes[i].name
            << " (" << nodes[i].gridX << "," << nodes[i].gridY << ") ";
        if (nodes[i].isAirport) {
            cout << "[Airport]";
        }
        cout << "\n   Edges: ";

        EdgeNode* e = nodes[i].head;
        while (e != nullptr) {
            cout << nodes[i].name << " -> " << e->name << "  Distance = " << e->weight << endl;
            e = e->next;
        }
        cout << "\n";
    }

    // print planes
    cout << "Planes (" << planeCount << "):\n";
    for (int i = 0; i < planeCount; ++i) {
        if (planes[i] != nullptr) {
            cout << "  " << planes[i]->flightId << " at node ";
            int nidx = planes[i]->currentNodeIndex;
            if (nidx >= 0 && nidx < nodeCount) cout << nodes[nidx].name;
            else cout << "(unknown)";
            cout << "\n";
        }
    }
}


void Airspace::printSafeRoute(const string& flightId)
{
    int start = getNodeIndexOfPlane(flightId);
    if (start == -1) {
        cout << "Error: Flight not found.\n";
        return;
    }

    cout << "\n> Calculate Safe Route for: " << flightId
        << " (at " << nodes[start].name << ")\n\n";

    cout << "CALCULATING PATH TO NEAREST AIRPORT...\n";
    cout << "--------------------------------------------\n";

    int path[MAX_NODES];
    int len = 0;

    int airportIndex = findNearestAirport(start, path, len);

    if (airportIndex == -1) {
        cout << "No reachable airport found.\n";
        return;
    }

    int totalDist = 0;

    // Collision checking along the route
    for (int i = 1; i < len; i++) {
        int nodeIndex = path[i];

        if (isNodeOccupied(nodeIndex)) {
            cout << "WARNING: Collision risk! Node '"
                << nodes[nodeIndex].name
                << "' is occupied.\n";
            cout << "SAFE ROUTE CANNOT BE GENERATED.\n";
            return;
        }
    }

    // Print Visual Path
    cout << "Start: " << nodes[path[0]].name << "\n";

    for (int i = 1; i < len; i++) {
        cout << "  |\n";
        cout << "  V\n";

        // find edge weight
        int w = 0;
        EdgeNode* e = nodes[path[i - 1]].head;
        while (e != nullptr) {
            if (e->to == path[i]) { w = e->weight; break; }
            e = e->next;
        }

        totalDist += w;

        cout << nodes[path[i]].name << " (Distance: " << w << "km)\n";
    }

    cout << "\nDestination: "
        << nodes[airportIndex].name
        << " (Total Dist: " << totalDist << "km)\n";

    cout << "--------------------------------------------\n";
}


