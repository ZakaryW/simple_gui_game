#ifndef LADDER_H
#define LADDER_H

#include "event.h"

class Ladder : public Event
{
    private:
    bool type;

    public:
    Ladder(bool);
    ~Ladder();
    void encounter();
    void percept() const;
    void print_event() const;
    bool get_type() const;
    string get_percept() const;
    string get_encounter() const;
};


#endif