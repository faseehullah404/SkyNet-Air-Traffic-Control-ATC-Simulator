#include<iostream>
#include"History.h"

using namespace std;
Node::Node(Flight *f, string ti): f(f),time(ti)  {
	left = right = nullptr;
	height = 1;

}
History::History() {
    root = nullptr;
    hfiles.open("avl.txt",ios::in);
    string id, origin, dest, status,time;
    int priority, fuel;
    while (hfiles >> id >> origin >> dest >> fuel >> priority >> status >> time) {
        Flight* f = new Flight(id, (FlightPriority)priority, fuel, origin, dest, status);
        insertFlight(f, time, true);
    }

}
Node* History::insert(Node* node, Flight* f, string time) {

   
    if (node == nullptr) {
        return new Node(f, time);
    }

    if (time < node->time) {
        node->left = insert(node->left, f, time);
    }
    else if (time > node->time) {
        node->right = insert(node->right, f, time);
    }
    else {
        return node;
    }

    
    node->height = 1 + max(getHeight(node->left),
        getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 && time < node->left->time) {
        return rightRotate(node);
    }

    if (balance < -1 && time > node->right->time) {
        return leftRotate(node);
    }
    
    if (balance > 1 && time > node->left->time) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

 
    if (balance < -1 && time < node->right->time) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node; 
}

void History::insertFlight(Flight* f, string time, bool ff) {
    if (!ff) {
        fstream hfile;
        hfile.open("avl.txt", ios::app);
        hfile << f->flightId << " " << f->origin << " " << f->destination << " " << f->fuelPercent << " " << f->priority << " " << f->status << " " << time << endl;
        hfile.close();
        cout << "Inserting "<< f->flightId<<"\n";
        root = insert(root, f, time);
    }
    else {
        root = insert(root, f, time);
    }
	
    
}

int History::getHeight(Node* n) {
    if (n == nullptr) return 0;
    return n->height;
}

int History::getBalance(Node* n) {
    if (n == nullptr) return 0;
    return getHeight(n->left) - getHeight(n->right);
}

Node* History::rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

   
    x->right = y;
    y->left = T2;

 
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x; 
}

Node* History::leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y; 
}

void History::inorder(Node* node) {
    if (node == nullptr) {
        return;
    }
    inorder(node->left);
    cout << "Flight ID: " << node->f->flightId << "\nDestination: " << node->f->destination << "\nFuel: " << node->f->fuelPercent << "\nStatus: " << node->f->status << "\n";
    inorder(node->right);
}

void History::printHistory() {
    inorder(root);
}

