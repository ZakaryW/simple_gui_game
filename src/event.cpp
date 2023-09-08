#include "event.h"

#include <iostream>

using namespace std;

//Event Implementation

Event::Event(int setid) : id(setid) {};
Event::Event() : id(VoidID) {};
Event::~Event(){};

int Event::get_id() const { return this->id; }