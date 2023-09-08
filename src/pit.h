#ifndef PIT_H
#define PIT_H 

#include "event.h"

class Pit : public Event
{
    private:

    public:
        Pit();
        ~Pit();
        void encounter();
        void percept() const;
        void print_event() const;
        string get_percept() const;
        string get_encounter() const;

};

#endif