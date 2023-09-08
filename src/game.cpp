#include "game.h"
#include "gold.h"
#include "pit.h"
#include "bats.h"
#include "wumpus.h"
#include "constants.h"

#include <iostream>

using namespace std;

//Game Implementation

Game::Game(){
	this->height = 0;
	this->length = 0;
	this->width = 0;
	
	this->win.gold = true;
	this->win.wumpus = true;
	this->win.alive = true;
	this->win.x = 0;
	this->win.y = 0;
	this->win.z = 0;

	this->player.gold = false;
	this->player.wumpus = false;
	this->player.alive = true;
	this->player.x = 0;
	this->player.y = 0;
	this->player.z = 0;

	this->reverse = 0;
	this->saveLoaded = false;
	this->climbLadder = false;
}

Game::Game(const Game &obj)
{
	this->height = obj.height;
	this->length = obj.length;
	this->width = obj.width;
	this->num_arrows = obj.num_arrows;
	this->debug_view = obj.debug_view;
	this->reverse = obj.reverse;
	this->saveLoaded = obj.saveLoaded;
	this->player.alive = obj.player.alive;
	this->player.gold = obj.player.gold;
	this->player.wumpus = obj.player.wumpus;
	this->player.x = obj.player.x;
	this->player.y = obj.player.y;
	this->player.z = obj.player.z;
	this->maps = obj.maps;
	this->climbLadder = obj.climbLadder;
}

Game& Game::operator=(const Game &obj)
{
	this->height = obj.height;
	this->length = obj.length;
	this->width = obj.width;
	this->num_arrows = obj.num_arrows;
	this->debug_view = obj.debug_view;
	this->reverse = obj.reverse;
	this->saveLoaded = obj.saveLoaded;
	this->player.alive = obj.player.alive;
	this->player.gold = obj.player.gold;
	this->player.wumpus = obj.player.wumpus;
	this->player.x = obj.player.x;
	this->player.y = obj.player.y;
	this->player.z = obj.player.z;
	this->maps = obj.maps;
	this->climbLadder = obj.climbLadder;

	return *this;
}

Game::~Game(){
};

bool Game::is_save_loaded() const
{
	return this->saveLoaded;
}

void Game::prompt_load_save()
{
	char input;
	if(saveLoaded)
	while(1){
		cout << "Do you want to load the same map? (y/n): ";
		cin >> input;
		cout << endl;

		if(input == 'y')
			break;
		
		if(input == 'n'){
			this->saveLoaded = false;
			break;
		}
		cout << "Error... invalid entry." << endl;
	}
	return;
}

void Game::set_up(int l, int w, int h){
	//set up the game
	this->length = l;
	this->width = w;
	this->height = h;
	this->maps.set_dimensions(this->length, this->width, this->height);

	this->num_arrows = 3; 	//start with 3 arrows
	for(int x = 0; x < h; ++x){
		vector<vector<Room> > newfloor;
		for(int i = 0; i < l; ++i){
			vector<Room> row;
			for(int j = 0 ; j < w; ++j)
				row.push_back(Room());
			newfloor.push_back(row);
		}
		maps.store_floor(newfloor);
	}
	this->maps.generate_bats();
	this->maps.generate_pits();
	this->maps.generate_ladders();
	this->maps.generate_gold();
	this->maps.generate_wumpus();

	this->game_map = maps.get_floor(player.z);
	Game::spawn_player();
	return;
}

void Game::spawn_player()
{
	while(1)
	{
		int row = rand() % this->length;
		int column = rand() % this->width;

		//The player shouldn't spawn in the same location as an event
		if(this->game_map[row][column].get_event() != NULL)
			continue;
		
		this->player.x = row;
		this->player.y = column;
		this->player.z = 0;

		this->win.x = this->player.x;
		this->win.y = this->player.y;
		this->win.z = this->player.z;
		break;
	}
}

void Game::display_game() const{
	cout << endl << endl;
	cout << "Arrows remaining: " << this->num_arrows << endl;
	
	string line = "";
	for (int i = 0; i < this->width; ++i)
		line += "-----";

	for (int i = 0; i < this->length; ++i)
	{
		cout << line << endl;
		for (int j = 0; j < this->width; ++j)
		{
			if(i != player.x || j != player.y)
				cout << " ";
			else
				cout << "*";

			//Print "  ||" if the room has no event
			if(this->game_map[i][j].get_event() == NULL)
				cout << "  ||";
			//Otherwise check if the debug mode is true (enabled), if so print the corresponding event symbol, if not event stays hidden
			else{
				if(this->debug_view)
					this->game_map[i][j].print_room_event();
				else if(this->player.x == i && this->player.y == j)
					this->game_map[i][j].print_room_event();
				else
					cout << " ";
				cout << " ||";
			}
		}
		cout << endl;
	}
	cout << line << endl;
}

void Game::encounter_check()
{
	if(this->game_map[player.x][player.y].get_event() != NULL)
		switch(this->game_map[player.x][player.y].room_encounter())
		{
			case WumpID:
			this->player.alive = false;//Player has died
			break;
			case GoldID:
			this->player.gold = true;	//Player has aquired the gold
			this->maps.clear_room(player.z, player.x, player.y);
			break;
			case BatsID:
			this->reverse = 5;	//Reverse input for 5 rounds
			break;
			case PitID:
			this->player.alive = false;
			break;
			case LaddID:
			this->maps.set_floor(this->player.z, this->game_map);
			this->player.z--;
			break;
			case LaddUID:
			this->maps.set_floor(this->player.z, this->game_map);
			this->player.z++;
			break;
			default:
			break;
		}
	return;
}

bool Game::check_win() const{
	//Nested conditions not indented for better clarity
	if(this->player.alive == this->win.alive)
	if(this->player.gold == this->win.gold)
	if(this->player.wumpus == this->win.wumpus)
	if(this->player.x == this->win.x)
	if(this->player.y == this->win.y)
	if(this->player.z == this->win.z){
		return true; //If all above conditions are true, return true
	}

	return false;	//If atleast one of the conditions is false, chain breaks so return false
}

char Game::get_dir(){
	//get direction of arrow:
	char dir;
	const char valid[] = {'w', 'W', 'a', 'A', 's', 'S', 'D', 'd'};
	while(1){
		int i;
		cout << "Fire an arrow...." << endl;
		cout << "W-up" << endl;
		cout << "A-left" << endl;
		cout << "S-down" << endl;
		cout << "D-right" << endl;
	

		cout << "Enter direction: " << endl;
		cin >> dir;
		cin.ignore(256, '\n');

		//Checking if the input is valid via matching to array, checks through the null character to setup next conditional
		for(i = 0; i <= 8; i++)
			if(dir == valid[i])
				break;
		//If there was a valid match, the loop would break and thus i will not reach 8
		if(i < 8)
			break;
		//Otherwise no valid match so the input loop needs to execute again
		cout << "Invalid input... try again." << endl;
	}
	//Standardizing the input to lowercase for easier handling
	return tolower(dir);
}

void Game::wumpus_move()
{
	if(this->player.wumpus) //If the wumpus is dead then it cannot move
		return;

	int random = rand() % 3;
	int tmp;
	//Unless a 0 is generated, the move will execute
	if(random > 0){
		this->maps.remove_wumpus();
		this->maps.generate_wumpus();
		cout << "The Wumpus has fled to a new location!" << endl;
		return;
	}
	cout << "The Wumpus did not move." << endl;
	return;
}

void Game::fire_arrow()
{
	char dir = get_dir();
	if(this->player.wumpus){
		this->num_arrows--;
		return;
	}
	switch(dir)
	{
		case 'w':
		this->player.wumpus = arrow_north(this->player.x, this->player.y);
		this->num_arrows--;
		break;
		case 'a':
		this->player.wumpus = arrow_west(this->player.x, this->player.y);
		this->num_arrows--;
		break;
		case 's':
		this->player.wumpus = arrow_south(this->player.x, this->player.y);
		this->num_arrows--;
		break;
		case 'd':
		this->player.wumpus = arrow_east(this->player.x, this->player.y);
		this->num_arrows--;
		break;
	}

	if(this->player.wumpus){	//The wumpus has been slain, and condition is set so return
		for(int i = 0; i < this->length; ++i)
			for(int j = 0; j < this->width; ++j)
				if(this->game_map[j][i].check_for_wumpus())
					this->game_map[j][i].delete_event();

		cout << "Wumpus has been slain!" << endl;
		this->maps.set_floor(player.z, this->game_map);
		return;
	}
	cout << "You missed." << endl;

	Game::wumpus_move();

	return;
}

bool Game::arrow_north(const int row, const int col)
{
	for(int i = row; i >= 0; --i){
		if(this->game_map[i][col].check_for_wumpus())
			return true;
		if(i == row - 3)
			break;
	}
	return false;
}

bool Game::arrow_south(const int row, const int col)
{
	for(int i = row; i < this->length; ++i){
		if(this->game_map[i][col].check_for_wumpus())
			return true;
		if(i == row + 3)
			break;
	}
	return false;
}

bool Game::arrow_east(const int row, const int col)
{
	for(int i = col; i < this->width; ++i){
		if(this->game_map[row][i].check_for_wumpus())
			return true;
		if(i == col + 3)
			break;
	}
	return false;
}

bool Game::arrow_west(const int row, const int col)
{
	for(int i = col; i > 0; --i){
		if(this->game_map[row][i].check_for_wumpus())
			return true;
		if(i == col - 3)
			break;
	}
	return false;
}

char Game::apply_reverse(char c)
{
	switch(c)
	{
		case 'w':
		return 's';
		case 's':
		return 'w';
		case 'a':
		return 'd';
		case 'd':
		return 'a';
	}
	return c;
}

void Game::move(char c) 
{
	if (c == 'f'){
		if(this->num_arrows <= 0){
			cout << "You are out of arrows..." << endl;
			return;
		}
		Game::fire_arrow();
		return;
	}
	if(c == 'e'){
		this->climbLadder = true;
		return;
	}
	if(this->reverse > 0){
		c = Game::apply_reverse(c);
		this-reverse--;
	}
	switch(c){
		case 'w':
			Game::move_up();
			break;
		case 'a':
			Game::move_left();
			break;
		case 's':
			Game::move_down();
			break;
		case 'd':
			Game::move_right();
			break;
	}
	this->maps.set_floor(player.z, this->game_map);
	return;
}

void Game::move_up() 
{
	if(this->player.x == 0)
		return;
	
	this->player.x--;
	return;
}

void Game::move_down() 
{
	if(this->player.x == this->length-1)
		return;
	
	this->player.x++;
	return;
}

void Game::move_left() 
{
	if(this->player.y == 0)
		return;
	
	this->player.y--;
	return;
}

void Game::move_right() 
{
	if(this->player.y == this->width-1)
		return;
	
	this->player.y++;
	return;
}

char Game::get_input()
{
	//get action, move direction or firing an arrow
	char c;
	const char valid[] = {'w', 'W', 'a', 'A', 's', 'S', 'D', 'd', 'f', 'F'};
	while(1){
		int i;
		cout << endl << endl << "Player move..." << endl << endl;
		cout << "W-up" << endl;
		cout << "A-left" << endl;
		cout << "S-down" << endl;
		cout << "D-right" << endl;
		cout << "f-fire an arrow" << endl;

		cout << "Enter input: " << endl;
		cin >> c;
		cin.ignore(256, '\n');

		//Checking if the input is valid via matching to array, checks through the null character to setup next conditional
		for(i = 0; i <= 10; i++)
			if(c == valid[i])
				break;
		//If there was a valid match, the loop would break and thus i will not reach 10 otherwise no valid match so the input loop needs to execute again
		if(i < 10)
			break;
		cout << "Invalid input... try again." << endl;
	}
	return tolower(c);	//The input is standardized by always returning in lower case
}

void Game::display_percepts() const
{
	int x = this->player.x, y = this->player.y;

	if(x < this->length - 1)
		if(this->game_map[x+1][y].get_event() != NULL)
			this->game_map[x+1][y].room_percept();

	if(x > 0)
		if(this->game_map[x-1][y].get_event() != NULL)
			this->game_map[x-1][y].room_percept();

	if(y < this->width - 1)
		if(this->game_map[x][y+1].get_event() != NULL)
			this->game_map[x][y+1].room_percept();

	if(y > 0)
		if(this->game_map[x][y-1].get_event() != NULL)
			this->game_map[x][y-1].room_percept();

	return;
}
void Game::create_instance(int w, int l, int h, bool d)
{
	if(this->saveLoaded)
		return;

	Game::set_up(l, w, h);
	
	this->length = l;
	this->width = w;
	this->height = h;
	this->debug_view = d;

	return;
}

void Game::play_game(int w, int l, int h, bool d)
{	
	if(!saveLoaded)
		Game::set_up(l, w, h);
	
	this->length = l;
	this->width = w;
	this->height = h;
	this->debug_view = d;
	Game backup = *this;


	while (Game::check_win() == false){

		
		this->game_map = maps.get_floor(player.z);
		
		//Game::display_game();

		//display event notifications
		//Game::display_percepts();

		//get input
		//input = Game::get_input();

		//player moves
		//Game::move(input);
		

		//check for event encounters
		//Game::encounter_check();

		if(!this->player.alive)
			break;

		
	}
	if(!player.alive){
		*this = backup;
		this->saveLoaded = true;
	}
	return;
}

vector<string> Game::get_percepts() const
{
	vector<string> tmp;

	if(this->player.x < this->length - 1)
		if(this->game_map[this->player.x+1][this->player.y].get_event() != NULL)
			tmp.push_back(this->game_map[this->player.x+1][this->player.y].get_event()->get_percept());

	if(this->player.x > 0)
		if(this->game_map[player.x-1][player.y].get_event() != NULL)
			tmp.push_back(this->game_map[this->player.x-1][this->player.y].get_event()->get_percept());

	if(this->player.y < this->width - 1)
		if(this->game_map[player.x][player.y+1].get_event() != NULL)
			tmp.push_back(this->game_map[this->player.x][this->player.y+1].get_event()->get_percept());

	if(this->player.y > 0)
		if(this->game_map[player.x][player.y-1].get_event() != NULL)
			tmp.push_back(this->game_map[this->player.x][this->player.y-1].get_event()->get_percept());

	return tmp;
}

void Game::update_map()
{
	this->game_map = maps.get_floor(player.z);
	return;
}

vector<vector<Room> > Game::get_map() const {	return this->game_map;	}

Tasks Game::get_player_data() const {	return this->player;	}

string Game::get_encounter_message() const
{
	return this->game_map[player.x][player.y].get_event_notification();
}

void Game::process_encounter()
{
	if(this->game_map[player.x][player.y].get_event() == NULL)
		return;
	
	switch(this->game_map[player.x][player.y].get_event()->get_id())
	{
		case WumpID:
		this->player.alive = false;//Player has died
		break;
		case GoldID:
		this->player.gold = true;	//Player has aquired the gold
		this->maps.clear_room(player.z, player.x, player.y);
		break;
		case BatsID:
		this->reverse = 5;	//Reverse input for 5 rounds
		break;
		case PitID:
		this->player.alive = false;
		break;
		case LaddID:
		if(this->climbLadder){
			this->maps.set_floor(this->player.z, this->game_map);
			this->player.z--;
		}
		break;
		case LaddUID:
		if(this->climbLadder){
			this->maps.set_floor(this->player.z, this->game_map);
			this->player.z++;
		}
		break;
		default:
		break;
	}

	this->climbLadder = false;
	return;
}

bool Game::player_alive() const { return this->player.alive; }

void Game::set_save_loaded(const bool saveLoaded)
{
	this->saveLoaded = saveLoaded;
}