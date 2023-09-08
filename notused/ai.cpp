#include "ai.h"

using namespace std;

void AI::process_percepts(const vector<vector<Room> >currMap, const int x, const int y, const int z)
{

    if(x < currMap.size() - 1)
		if(currMap[x+1][y].get_event() != NULL){
            point_interest p = {currMap[x+1][y].get_event(), x+1, y, z};
            this->pois.push_back(p);
        }

	if(x > 0)
		if(currMap[x-1][y].get_event() != NULL){
            point_interest p = {currMap[x-1][y].get_event(), x-1, y, z};
            this->pois.push_back(p);
        }

	if(y < currMap[0].size() - 1)
		if(currMap[x][y+1].get_event() != NULL){
            point_interest p = {currMap[x][y+1].get_event(), x, y+1, z};
            this->pois.push_back(p);
        }

	if(y > 0)
		if(currMap[x][y-1].get_event() != NULL){
            point_interest p = {currMap[x][y-1].get_event(), x, y-1, z};
            this->pois.push_back(p);
        }
}

void AI::generate_decisions()
{
    for(int i = 0; i < this->pois.size(); ++i)
        if(pois[i].e->get_id() == WumpID){
            moveSeq.push_back(AI::generate_direction());
            moveSeq.push_back('f');
            pois.pop_back();
            return;
        }
    for(int i = 0; i < this->pois.size(); ++i){
        moveSeq.push_back(AI::generate_direction());
        pois.pop_back();
        return;
    }
    
    return;
}

char AI::generate_direction()
{
    int random = rand() % 4;

    switch(random)
    {
        case 0:
        return 'w';
        case 1:
        return 'a';
        case 2:
        return 's';
        case 3:
        return 'd';
        default:
        return 'w';
    }
}

char AI::AI_move()
{
    
}