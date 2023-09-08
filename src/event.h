#ifndef EVENT_H
#define EVENT_H 

#include "constants.h"
#include <iostream>
#include <string>

using namespace std;

//Event Interface
//Note: this must be an abstract class!

class Event
{

protected:
	const int id;

public:
    Event(int);
    Event();
    virtual ~Event();
	virtual void percept() const = 0;
    virtual void encounter() = 0;
	virtual void print_event() const = 0;
    virtual string get_percept() const = 0;
    virtual string get_encounter() const = 0;
    int get_id() const;
};
#endif