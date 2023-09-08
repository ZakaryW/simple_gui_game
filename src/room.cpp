#include "room.h"

using namespace std;

Room::Room()
{
    this->event = NULL;
}

Room& Room::operator=(const Room& obj)
{
    if(this->event != NULL)
        delete this->event;

    if(obj.event == NULL)
        this->event = NULL;
    else
        switch(obj.event->get_id())
        {
            case WumpID:
            this->event = new Wumpus();
            break;
            case GoldID:
            this->event = new Gold();
            break;
            case BatsID:
            this->event = new Bats();
            break;
            case PitID:
            this->event = new Pit();
            break;
            case LaddID:
            this->event = new Ladder(0);
            break;
            case LaddUID:
            this->event = new Ladder(1);
            break;

        }
    return *this;
}

Room::Room(const Room& obj)
{
    if(obj.event == NULL)
        this->event = NULL;
    else
        switch(obj.get_event()->get_id())
        {
            case WumpID:
            this->event = new Wumpus();
            break;
            case GoldID:
            this->event = new Gold();
            break;
            case BatsID:
            this->event = new Bats();
            break;
            case PitID:
            this->event = new Pit();
            break;
            case LaddID:
            this->event = new Ladder(0);
            break;
            case LaddUID:
            this->event = new Ladder(1);
            break;
        }
}

Room::~Room()
{
    if(this->event != NULL)
        delete this->event;

    this->event = NULL;
}

void Room::create_gold_event()
{
    this->event = new Gold();
    return;
}

void Room::create_pit_event()
{
    this->event = new Pit();
    return;
}

void Room::create_bat_event()
{
    this->event = new Bats();
    return;
}   

void Room::create_wumpus_event()
{
    this->event = new Wumpus();
    return;
}

void Room::create_ladder_event(bool type)
{
    this->event = new Ladder(type);
}

Event* Room::get_event() const {    return this->event;    }

void Room::delete_event()
{
    if(this->event != NULL){
        delete this->event;
        this->event = NULL;
    }
    return;
}

bool Room::check_for_wumpus() const
{
    if(this->event != NULL){
        if(this->event->get_id() == WumpID)
            return true;
    }
    return false;
}

void Room::room_percept() const
{
    if(this->event != NULL)
        this->event->percept();
    
    return;
}

int Room::room_encounter() const
{
    if(this->event != NULL)
    {
        this->event->encounter();
        return this->event->get_id();
    }
    return VoidID;
}

void Room::print_room_event() const
{   //Double checking for event's existence
    if(this->event != NULL)
        this->event->print_event();
    return;
}

string Room::get_event_notification() const
{
    if(this->event != NULL)
        return this->event->get_encounter();
    else
        return "";
}