#include "ladder.h"

using namespace std;

Ladder::Ladder(bool type) : Event(LaddID + type) {
    this->type = type;
};

Ladder::~Ladder() {};

void Ladder::percept() const
{
    cout << "You can make out the silhouette of a ladder." << endl;
    return;
}

void Ladder::encounter()
{
    cout << "You found a ladder. ";
    if(this->type)
        cout << "You ascended a level." << endl;
    else
        cout << "You descended a level." << endl;
    
    return;
}

void Ladder::print_event() const
{
    if(this->type)
        cout << "^";
    else
        cout << "v";
    return;
}

bool Ladder::get_type() const { return this->type;  }

string Ladder::get_percept() const { return "You can make out the silhouette of a ladder."; }

string Ladder::get_encounter() const
{
    string tmp = "You found a ladder. ";
    if(this->type)
        tmp += "Press (e) to ascended a level.";
    else
        tmp += "Press (e) descended a level";
    
    return tmp;
}