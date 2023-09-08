#ifndef AI_H
#define AI_H

#include "game.h"


//Implement as an alternated state of the game
//Check for percepts
//If percepts are detected:
//  generate a sequence of moves dependent on the percept(s) detected
//  if mulitple percepts are detected, move precedence should be dependent on the level of importance of the nearby events
//If no percepts are detected a random move will be generated instead
//Keep track of each move and chart what's known about the map

struct point_interest{
    Event *e;
    int poiX;
    int poiY;
    int poiZ;
};

class AI : public Game {

private:
    vector<char> moveSeq;
    vector<point_interest> pois;

public:
    void process_percepts(const vector<vector<Room> >currMap, const int x, const int y, const int z);
    void generate_decisions();
    char generate_direction();

    char AI_move();

};

#endif