#ifndef WUMPUS_H
#define WUMPUS_H 

#include "event.h"

class Wumpus : public Event
{
    private:

    public:
        Wumpus();
        ~Wumpus();
        void encounter();
        void percept() const;
        void print_event() const;
        string get_percept() const;
        string get_encounter() const;
};

#endif