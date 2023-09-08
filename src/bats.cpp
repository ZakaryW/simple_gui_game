#include "bats.h"

#include <iostream>

using namespace std;

//Bats Implementation

Bats::Bats() : Event(BatsID) {};
Bats::~Bats(){};

void Bats::encounter()
{
    cout << "Bats surround you. You have become disorientated for 5 turns." << endl;
    return;
}

void Bats::percept() const
{
    cout << "You hear wings flapping." << endl;
    return;
}

void Bats::print_event() const
{
    cout << "<";
    return;
}

string Bats::get_percept() const
{
    return "You hear wings flapping.";
}

string Bats::get_encounter() const
{
    return "Bats surround you. You have become disorientated for 5 turns.";
}