#ifndef BATS_H
#define BATS_H 

#include "event.h"

//Bats Interface
class Bats : public Event
{
    private:

    public:
        Bats();
        ~Bats();
        void encounter();
        void percept() const;
        void print_event() const;
        string get_percept() const;
        string get_encounter() const;

};

#endif