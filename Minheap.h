#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <string>
#include<fstream>
#include "Flight.h"
#include"Airspace.h"
using namespace std;


const int MAX_HEAP_SIZE = 100;

class MinHeap {
private:
    Flight* heap[MAX_HEAP_SIZE];
    int size;
    fstream hfile;
    int parent(int i) const;
    int left(int i) const;
    int right(int i) const;

    void heapifyDown(int i);
    void heapifyUp(int i);

    int comparePriority(const Flight* a, const Flight* b) const;

    int findIndexByFlightId(const string& id) const;

public:
    MinHeap();

    bool isEmpty() const;
    bool isFull() const;
    int getSize() const;
    void insertFromFile(Airspace& a, Flight *f);
    void insert(Flight* flight);
    Flight* getMin() const;
    Flight* extractMin();
    Flight* top();
    void insertF(Airspace& a);
    void changePriority(const string& flightId, FlightPriority newPriority);

    void printHeap() const;
};


#endif
