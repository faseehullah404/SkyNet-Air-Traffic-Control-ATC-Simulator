#ifndef HISTORY_H
#define HISTORY_H
#include <fstream>       
#include <string>
#include "Flight.h"
using namespace std;
class Node {
public:
    Flight* f;
    string time;
    Node* left;
    Node* right;
    int height;
   
    Node(Flight* flight, string t);
};

class History {
private:
	Node* root;
    fstream hfiles;
	Node* insert(Node* node, Flight *f, string t);
    int getHeight(Node*);
    int getBalance(Node*);
    Node* rightRotate(Node*);
    Node* leftRotate(Node*);

    void inorder(Node* node);
public:
    History();

    void printHistory();
    void insertFlight(Flight* f, string t, bool fromFile = false);
};
#endif