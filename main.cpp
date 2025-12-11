#include <iostream>
#include "Airspace.h"
#include "MinHeap.h"
#include "Flight.h"
#include "Registry.h"
#include "History.h"
#include <cstdlib>
#include <ctime>
using namespace std;

void printMenu() {
    cout << "\n================ SKYNET ATC SIMULATOR ================\n";
    cout << "1. Show Airspace Radar\n";
    cout << "2. Add Airport\n";
    cout << "3. Add Waypoint\n";
    cout << "4. Add Edge \n";
    cout << "5. Add New Flight\n";
    cout << "6. Declare Emergency\n";
    cout << "7. Land Next Plane\n";
    cout << "8. Move Plane to Node\n";
    cout << "9. Print Graph\n";
    cout << "10. Find Nearest Airport\n";
    cout << "11. Search Flight\n";
    cout << "12. View Flights Log Lists\n";
    cout << "13. Calculate Safe Route\n";
    cout << "0. Exit\n";
    cout << "=======================================================\n";
    cout << "Choose option: ";
}

int main() {

    Airspace air;
    MinHeap heap;
    Registry registerf;
    History h;
    registerf.loadFlightInAirspace(air);
    heap.insertF(air);
    int choice = 1;
    string fId;
    string landTime;
    //srand(time(0)); // for random IDs if needed
    //================+TESTING+====================================++
     // your hash table object

    // Create and register multiple flights
    //for (int i = 1; i <= 20; ++i) {
    //    string id = "PK-" + to_string(100 + i); // PK-101, PK-102, ...
    //    int fuel = 10 + rand() % 91; // random fuel 10-100%
    //    FlightPriority pr = (FlightPriority)((rand() % 4) + 1); // random priority 1-4
    //    Flight* f = new Flight(id, pr, fuel, "KAR", "LHR");
    //    registerf.RegisterFlight(f);
    //}

    //// Optionally add some manually for known IDs
    //Flight* f1 = new Flight("PK-200", HIGH, 15, "KAR", "LHR");
    //registerf.RegisterFlight(f1);
    //Flight* f2 = new Flight("PK-201", MEDIUM, 50, "KAR", "LHR");
    //registerf.RegisterFlight(f2);
    //Flight* f3 = new Flight("PK-202", CRITICAL, 5, "KAR", "LHR");
    //registerf.RegisterFlight(f3);

    //cout << "\n--- All Flights Registered ---\n";

    //// Search for an existing flight
    //cout << "\nSearching for PK-106:\n";
    //registerf.searchFlight("PK-106"); // should be found

    //// Search for a flight that does NOT exist
    //cout << "\nSearching for PK-999:\n";
    //registerf.searchFlight("PK-999"); // should NOT be found
    string status;
    while (choice) {
        printMenu();
        cin >> choice;
        system("cls");
        switch (choice) {

        case 0: {
            cout << "Exiting SkyNet ATC System...\n";
            break;
        }

        case 1: {
            air.updateRadar();
            air.printRadar();
            heap.printHeap();
            break;
        }

        case 2: {
            string name;
            int x, y;
            cout << "Enter airport name: ";
            cin >> name;
            cout << "Enter gridX gridY: ";
            cin >> x >> y;
            air.addAirport(name, x, y);
            break;
        }

        case 3: {
            string name;
            int x, y;
            cout << "Enter waypoint name: ";
            cin >> name;
            cout << "Enter gridX gridY: ";
            cin >> x >> y;
            air.addWaypoint(name, x, y);
            break;
        }

        case 4: {
            string a, b;
            int w;
            cout << "From node: ";
            cin >> a;
            cout << "To node: ";
            cin >> b;
            cout << "Weight (distance): ";
            cin >> w;
            air.addEdge(a, b, w);
            break;
        }

        case 5: {
            string id, from, to, startPort;
            int fuel;
            int pri;

            cout << "Enter Flight ID: ";
            cin >> id;

            cout << "Enter origin: ";
            cin >> from;

            cout << "Enter destination: ";
            cin >> to;

            cout << "Enter fuel percent: ";
            cin >> fuel;

            cout << "Enter priority (1=Critical, 2=High, 3=Medium, 4=Low): ";
            cin >> pri;

            cout << "Enter start node name: ";
            cin >> startPort;

            switch (pri) {
            case 1:
                status = "Critical";
                break;
            case 2:
                status = "Emergency";
                break;
            case 3:
                status = "LowEmergency";
                break;
            case 4:
                status = "Normal";
                break;
            default:
                cout << "Invalid Priority! Flight Status set to Normal!\n";
                status = "Normal";
                break;
            }

            Flight* f = new Flight(id, (FlightPriority)pri, fuel, from, to, status);
            if (air.isFlightExist(id)) {
                cout << "Flight Already Exists!\n";
                break;
            }
            if (air.addPlaneToAirspace(f, startPort)) {
                heap.insert(f);
                cout << "Flight added successfully.\n";
                registerf.RegisterFlight(f);

            }
            else {
                cout << "Failed to add flight to airspace.\n";
                
            }

            break;
        }

        case 6: {
            string id;
            int np;
            cout << "Enter Flight ID: ";
            cin >> id;
            np = 1;

            heap.changePriority(id, (FlightPriority)np);

            cout << "Emergency for flight "<<id<<" Declared.\n";
            break;
        }

        case 7: {
            Flight* top = heap.getMin();
            if (!air.isAirportAtIndex(top->currentNodeIndex) && (top->status != "Emergency" || top->status != "Critical")) {
                cout << "Please Reach at airport to land!\n";
                break;
            }
            Flight* landing = heap.extractMin();
            
            if (!landing) {
                cout << "No planes in landing queue.\n";
            }
            else {
                cout << "Landing plane: " << landing->flightId << endl;
                cout << "Enter Status of Landing: ";
                cin.ignore();
                getline(cin, status);
                landing->status = status;
                cout << "Please Write Landing Time: (830)";
                cin >> landTime;
                
                h.insertFlight(landing, landTime);
                if (air.removePlaneFromAirspace(landing->flightId)) {
                    cout << "Flight Landed Successfully at " << air.getNameByIndex(landing->currentNodeIndex)<<"\n";
                }
                else {
                    cout << "Problem in landing flight!\n";
                }
               
            }
            break;
        }

        case 8: {
            string id, node;
            cout << "Enter Flight ID: ";
            cin >> id;
            cout << "Enter destination node name: ";
            cin >> node;

            if (air.movePlaneToNode(id, node)) {
                cout << "Plane moved to "<<node<<" successfully.\n";
            }
            else {
                cout << "Move failed (collision OR invalid node).\n";
            }
            break;
        }

        case 9: {
            air.printGraph();
            break;
        }

        case 10: {
            string id;
            cout << "Enter Flight ID: ";
            cin >> id;

            int startNode = air.getNodeIndexOfPlane(id);
            if (startNode == -1) {
                cout << "Flight not found.\n";
                break;
            }

            int path[50];
            int len = 0;

            int airportIndex = air.findNearestAirport(startNode, path, len);

            if (airportIndex == -1) {
                cout << "No reachable airport found.\n";
            }
            else {
                cout << "Nearest airport index: " << airportIndex << endl;
                cout << "Path: ";
                for (int i = 0; i < len; i++) {
                    cout << air.getNameByIndex(path[i]);
                    if (i < len - 1) cout << " -> ";
                }
                cout << endl;
            }

            break;
        }

        case 11:

            cout << "Enter Flight ID: ";
            cin >> fId;
            cout << "Searching In Database...  ";
            if (!air.isFlightExist(fId)) {
                cout << "Flight Does Not Exist!\n";
                break;
            }
            cout << " Found\n";
            registerf.searchFlight(fId);
            break;
        case 12:
            cout << "---------SKYNET FLIGHTS LOGS----------\n";
            h.printHistory();
            break;
        case 13: {
            string id;
            cout << "Enter Flight ID: ";
            cin >> id;

            air.printSafeRoute(id);
            break;
        }
        default:
            cout << "Invalid option. Try again.\n";
            break;
        }
    }
}
