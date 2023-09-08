#ifndef CAVE_H
#define CAVE_H

#include<iostream>
#include <vector>
#include "room.h"

using namespace std;

class Cave
{
private:
    vector<vector<vector<Room> > > cave;
    int length;
    int width;
    int height;

public:
    Cave();
    Cave(const Cave&);
    Cave& operator=(const Cave&);
    ~Cave();
    
    void set_floor(const int, const vector<vector<Room> >) ;

    void store_floor(const vector<vector<Room> >);
    
    void set_dimensions(const int, const int, const int);

    Room get_room(int, int, int) const;

    void clear_room(const int, const int, const int);

    void generate_bats();

    void generate_pits();

    void generate_gold();

    void generate_wumpus();

    void generate_ladders();

    void remove_wumpus();

    vector<vector<Room> > get_floor(const int) const;
};

#endif