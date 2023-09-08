#include "pit.h"

#include <iostream>

using namespace std;

//Pit Implementation

Pit::Pit() : Event(PitID) {};

Pit::~Pit() {};

void Pit::encounter()
{
    cout << "You stumble and fall down a pit. Game over." << endl;
    return;
}

void Pit::percept() const
{
    cout << "You feel a breeze." << endl;
    return;
}

void Pit::print_event() const
{
    cout << "O";
    return;
}

string Pit::get_percept() const
{
    return "You feel a breeze.";
}

string Pit::get_encounter() const { return "You stumble and fall down a pit. Game over."; }