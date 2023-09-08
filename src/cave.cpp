#include "cave.h"

using namespace std;

Cave::Cave() 
{
    this->height = 0;
    this->length = 0;
    this->width = 0;
};

Cave::Cave(const Cave &obj)
{   
    this->height = obj.height;
    this->length = obj.length;
    this->width = obj.width;
    this->cave = obj.cave;
}

Cave& Cave::operator=(const Cave &obj)
{
    for(int i = 0; i < this->height; ++i)
        for(int j = 0; j < this->length; ++j)
            for(int p = 0; p < this->width; ++p){
                if(this->cave[i][j][p].get_event() != NULL)
                    this->cave[i][j][p].delete_event();
            }
    
    this->height = obj.height;
    this->length = obj.length;
    this->width = obj.width;
    
    this->cave = obj.cave;

    return *this;
}

Cave::~Cave() 
{
    for(int i = 0; i < this->height; ++i)
        for(int j = 0; j < this->length; ++j)
            for(int p = 0; p < this->width; ++p){
                if(this->cave[i][j][p].get_event() != NULL)
                    this->cave[i][j][p].delete_event();
            }
};

void Cave::set_dimensions(const int len, const int wid, const int hei)
{
    this->length = len;
    this->width = wid;
    this->height = hei;
}

void Cave::set_floor(const int level, const vector<vector<Room> > map)
{
    for(int j = 0; j < this->length; ++j)
        for(int p = 0; p < this->width; ++p){
            if(this->cave[level][j][p].get_event() != NULL){
                cave[level][j][p].delete_event();
            }
            this->cave[level][j][p] = map[j][p];
        }
    return;
}

void Cave::store_floor(const vector<vector<Room> > map)
{
    this->cave.push_back(map);
    return;
}

Room Cave::get_room(int x, int y, int z) const { return this->cave[z][x][y]; }

void Cave::clear_room(const int z, const int x, const int y)
{
    if(this->cave[z][x][y].get_event() == NULL)
        return;
    
    this->cave[z][x][y].delete_event();
    return;
}

void Cave::generate_bats()
{
    for(int i = 0; i < this->height; ++i)
        for(int j = 0; j < 2; ++j){
            int x = rand() % this->length;
            int y = rand() % this->width;

            if(this->cave[i][x][y].get_event() != NULL)
                continue;
            
            this->cave[i][x][y].create_bat_event();
        }
    
    return;
}

void Cave::generate_pits()
{
    for(int i = 0; i < this->height; ++i)
        for(int j = 0; j < 2; ++j){
            int x = rand() % this->length;
            int y = rand() % this->width;

            if(this->cave[i][x][y].get_event() != NULL)
                continue;

            this->cave[i][x][y].create_pit_event();
        }
    
    return;
}

void Cave::generate_gold()
{
	while(1){
		int x = rand() % this->length;
		int y = rand() % this->width;
		int z = rand() % this->height;

		if(this->cave[z][x][y].get_event() != NULL)
			continue;

        this->cave[z][x][y].create_gold_event();
		break;
	}
    return;
}

void Cave::generate_wumpus()
{
	while(1){
		int x = rand() % this->length;
		int y = rand() % this->width;
		int z = rand() % this->height;

		if(this->cave[z][x][y].get_event() != NULL)
			continue;
		
		this->cave[z][x][y].create_wumpus_event();
		break;
	}
    return;
}

void Cave::generate_ladders()
{
    bool up = true, down = false;

    for(int i = 0; i < this->height - 1; ++i)
    {
        while(1){
            int x = rand() % this->length;
            int y = rand() % this->width;

            if(this->cave[i][x][y].get_event() != NULL || this->cave[i+1][x][y].get_event() != NULL)
                continue;
        
            this->cave[i][x][y].create_ladder_event(up);
            this->cave[i+1][x][y].create_ladder_event(down);
            break;
        }
    }
    return;
}

void Cave::remove_wumpus()
{
    for(int i = 0; i < this->height; ++i)
        for(int j = 0; j < this->length; ++j)
            for(int p = 0; p < this->width; ++p)
                if(this->cave[i][j][p].check_for_wumpus())
                    this->cave[i][j][p].delete_event();
    
    return;
}

vector<vector<Room> > Cave::get_floor(const int z) const {   return this->cave[z];   }
