#include "MinHeap.h"
#include <iostream>

using namespace std;

// ---------------------- PRIVATE HELPERS ------------------------

int MinHeap::parent(int i) const { return (i - 1) / 2; }
int MinHeap::left(int i)   const { return 2 * i + 1; }
int MinHeap::right(int i)  const { return 2 * i + 2; }

// Compare two Flight priority levels
int MinHeap::comparePriority(const Flight* a, const Flight* b) const
{
    if (!a || !b) return 0;

    // 1. Priority by urgency (smaller = higher priority)
    if (a->priority < b->priority) return -1;
    if (a->priority > b->priority) return 1;

    // 2. Lower fuel gets higher priority
    if (a->fuelPercent < b->fuelPercent) return -1;
    if (a->fuelPercent > b->fuelPercent) return 1;

    // 3. Lexicographic ID tiebreak
    if (a->flightId < b->flightId) return -1;
    if (a->flightId > b->flightId) return 1;

    return 0;
}

void MinHeap::heapifyDown(int i)
{
    while (true) {
        int l = left(i);
        int r = right(i);
        int smallest = i;

        if (l < size && comparePriority(heap[l], heap[smallest]) < 0)
            smallest = l;

        if (r < size && comparePriority(heap[r], heap[smallest]) < 0)
            smallest = r;

        if (smallest != i) {
            Flight* temp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = temp;
            i = smallest;
        }
        else
            break;
    }
}

void MinHeap::heapifyUp(int i)
{
    while (i > 0 && comparePriority(heap[i], heap[parent(i)]) < 0) {
        int p = parent(i);
        Flight* temp = heap[i];
        heap[i] = heap[p];
        heap[p] = temp;
        i = p;
    }
}

int MinHeap::findIndexByFlightId(const string& id) const
{
    for (int i = 0; i < size; ++i) {
        if (heap[i] && heap[i]->flightId == id)
            return i;
    }
    return -1;
}

// ---------------------- PUBLIC METHODS ------------------------

MinHeap::MinHeap() : size(0)
{
    for (int i = 0; i < MAX_HEAP_SIZE; ++i) {
        heap[i] = nullptr;
    }

   
}

bool MinHeap::isEmpty() const
{
    return size == 0;
}

bool MinHeap::isFull() const
{
    return size >= MAX_HEAP_SIZE;
}

int MinHeap::getSize() const
{
    return size;
}

void MinHeap::insert(Flight* flight)
{
    if (!flight) {
        cout << "Error: Cannot insert NULL flight.\n";
        return;
    }

    if (isFull()) {
        cout << "MinHeap is full. Cannot insert.\n";
        return;
    }

    if (findIndexByFlightId(flight->flightId) != -1) {
        cout << "Error: Duplicate flight insertion prevented ("
            << flight->flightId << ").\n";
        return;
    }
    int i = size++;
    heap[i] = flight;
    heapifyUp(i);
}
Flight* MinHeap::top() {
    return heap[0];
}
void MinHeap::insertF(Airspace& a) {
    hfile.open("flights.txt", ios::in);
    string id, origin, status, dest, startNode;
    int priority, fuel, i;

    while (hfile >> id >> origin >> dest >> fuel >> priority >> status >> startNode >> i) {

        Flight* f = new Flight(id, (FlightPriority)priority, fuel, origin, dest, status);
        
        f->currentNodeIndex = a.findNodeIndexByName(startNode);
        if (f->currentNodeIndex == -1) {
            cout << "Current Node Not Found!";
            break;
        }
        insertFromFile(a,f);
    }
}
void MinHeap::insertFromFile(Airspace& a, Flight *f) {
    if (!f) {
        cout << "Error: Cannot insert NULL flight.\n";
        return;
    }

    if (isFull()) {
        cout << "MinHeap is full. Cannot insert.\n";
        return;
    }

    if (findIndexByFlightId(f->flightId) != -1) {
        cout << "Error: Duplicate flight insertion prevented ("
            << f->flightId << ").\n";
        return;
    }
    int i = size++;
    heap[i] = f;
    heapifyUp(i);
}

Flight* MinHeap::getMin() const
{
    if (isEmpty()) return nullptr;
    return heap[0];
}

Flight* MinHeap::extractMin()
{
    if (isEmpty()) return nullptr;

    Flight* root = heap[0];

    heap[0] = heap[size - 1];
    heap[size - 1] = nullptr;

    size--;

    if (size > 0)
        heapifyDown(0);

    return root;
}

void MinHeap::changePriority(const string& flightId, FlightPriority newPriority)
{
    int idx = findIndexByFlightId(flightId);
    if (idx == -1) {
        cout << "changePriority error: Flight not found.\n";
        return;
    }

    FlightPriority oldPriority = heap[idx]->priority;
    heap[idx]->priority = newPriority;
    heap[idx]->status = "Emergency";
    // If priority improved (smaller number), move up
    if (newPriority < oldPriority)
        heapifyUp(idx);
    else if (newPriority > oldPriority)
        heapifyDown(idx);
}

void MinHeap::printHeap() const
{
    cout << "Landing Priority Queue (Min-Heap), size = " << size << ":\n";
    for (int i = 0; i < size; ++i) {
        if (heap[i]) {
            cout << i << ": "
                << heap[i]->flightId
                << " [priority=" << heap[i]->priority
                << ", fuel=" << heap[i]->fuelPercent << "%]\n";
            //cout << heap[i]->currentNodeIndex << endl;
        }
    }
}
