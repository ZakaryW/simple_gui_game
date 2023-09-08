#include "gold.h"

#include <iostream>

using namespace std;

Gold::Gold() : Event(GoldID)
{
}

Gold::~Gold(){};

void Gold::percept() const
{
    cout << "You see a shiny glimmer nearby." << endl;
    
    return;
}

void Gold::encounter()
{
    cout << "You aquire the gold!" << endl;

    return;
}

void Gold::print_event() const
{
    cout << "$";
}

string Gold::get_percept() const { return "You see a shiny glimmer nearby."; }

string Gold::get_encounter() const { return "You aquire the gold!"; }


