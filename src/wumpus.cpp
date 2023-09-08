#include "wumpus.h"

#include <iostream>

using namespace std;

//Wumpus Implementation

Wumpus::Wumpus() : Event(WumpID) {};
Wumpus::~Wumpus(){};

void Wumpus::encounter()
{
    cout << "The Wumpus awakens. Game over." << endl;
    return;
}

void Wumpus::percept() const
{
    cout << "You smell a terrible stench." << endl;
    return;
}

void Wumpus::print_event() const
{
    cout << "W";
    return;
}

string Wumpus::get_percept() const { return "You smell a terrible stench."; }

string Wumpus::get_encounter() const { return "The Wumpus awakens. Game over."; }