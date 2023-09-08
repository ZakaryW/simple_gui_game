#ifndef GAME_H
#define GAME_H 

#include <vector>
#include <iostream> 
#include "room.h"
#include "cave.h"


using namespace std;


struct Tasks {		//Conditions for a win:
	bool wumpus;	//True if wumpus is dead
	bool gold;		//True if gold is aquired
	bool alive;
	int x;			//Should match starting location
	int y;			//^
	int z;			//^
};

//Game interface 
class Game
{
private:
	vector <vector<Room> > game_map; //Current 'loaded' level
	Cave maps;						 //Holds floors of levels

	//other member variables:
	int length; 			//length of the board
	int width;  			//width of the board
	int height;				//height of the board
	int num_arrows; 		//keep track of number of errors remaining
	bool debug_view;		//debug mode or not
	bool saveLoaded;
	bool climbLadder;		//True if the player intendeds to climb a ladder

	int reverse;        //Number of turns to reverse the input
	Tasks player;		//Measures the player's completed tasked
	Tasks win;			//Win condition tasks (player's tasks will be compared to this)

public:

	Game();
	Game(const Game&);
	Game& operator=(const Game&);
	~Game();

	bool is_save_loaded() const;
	void prompt_load_save();
	void set_up(int, int, int);

	void generate_wumpus();
	void generate_gold();
	void generate_bats();
	void generate_pits();
	void generate_ladders(const int);

	//Spawns the player in a random location
	void spawn_player();

	void display_game() const;

	void encounter_check();

	bool check_win() const;
	char get_dir();
	void wumpus_move();

	void fire_arrow();
	bool arrow_north(const int, const int);
	bool arrow_south(const int, const int);
	bool arrow_east(const int, const int);
	bool arrow_west(const int, const int);

	void move_up();
	void move_down();
	void move_left();
	void move_right();
	char apply_reverse(char);
	void move(char);

	char get_input();

	void display_percepts() const;

	void play_game(int, int, int, bool);

	//  9/5/23 update: rewriting legacy functions to adapt for GUI  //

	vector<string> get_percepts() const;

	void create_instance(int, int, int, bool);

	void update_map();

	vector<vector<Room> > get_map() const;

	Tasks get_player_data() const;

	string get_encounter_message() const;

	void process_encounter();

	bool player_alive() const;

	void set_save_loaded(const bool);
};
#endif