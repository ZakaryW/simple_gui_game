#ifndef ROOM_H
#define ROOM_H 

#include "event.h"
#include "gold.h"
#include "bats.h"
#include "wumpus.h"
#include "pit.h"
#include "ladder.h"

using namespace std;

//Room Interface
//Note: Each room may be empty or has an event (bat, gold, pit, or wumpus);
//		Use event polymorphically

class Room
{
private: 
    Event *event;
	
public:
    Room();
    Room& operator=(const Room&);
    Room(const Room&);
    ~Room();
	
    Event* get_event() const;
    void delete_event();
    bool check_for_wumpus() const;
    void room_percept() const;
    int room_encounter() const;

    void create_gold_event();
    void create_pit_event();
    void create_bat_event();
    void create_wumpus_event();
    void create_ladder_event(bool);
    void print_room_event() const;
    string get_event_notification() const;

};

#endif