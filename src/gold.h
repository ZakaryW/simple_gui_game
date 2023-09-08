#ifndef GOLD_H
#define GOLD_H 

#include "event.h"

class Gold : public Event
{
    private:

    public:
        Gold();
        ~Gold();
    
        void encounter();
        void percept() const;
        void print_event() const;
        string get_percept() const;
        string get_encounter() const;
};

#endif