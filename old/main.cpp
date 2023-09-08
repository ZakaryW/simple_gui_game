#include <iostream>
#include <cstdlib>
#include <ctime>
#include "game.h"

using namespace std;

int length_input()
{
	int input;
	while(1){
		cout << "Please enter the dimension of the cave length, (should be 6 or greater): ";
		cin >> input;
		cout << endl;

		if(input >= 6 && input <= 20)
			return input;
		
		cout << "Error... invalid entry." << endl;
	}
}

int width_input()
{
	int input;
	while(1){
		cout << "Please enter the dimension of the cave width, (should be 6 or greater): ";
		cin >> input;
		cout << endl;

		if(input >= 6 && input <= 20)
			return input;
		
		cout << "Error... invalid entry." << endl;
	}
}

int height_input()
{
	int input;
	while(1){
		cout << "Please enter the dimension of the cave height. (should be 1 or greater): ";
		cin >> input;
		cout << endl;

		if(input >= 1 && input <= 10)
			return input;
		
		cout << "Error... invalid entry." << endl;
	}
}

bool debug_input()
{
	char input;
	while(1){
		cout << "Should the game be ran in debug mode? (y/n): ";
		cin >> input;
		cout << endl;

		if(input == 'y')
			return true;
		if(input == 'n')
			return false;

		cout << "Error... invalid entry." << endl;
	}
}

bool should_play_again()
{
	char input;

	while(1){
		cout << "Do you want to play again? (y/n): ";
		cin >> input;
		cout << endl;
		
		if(input == 'y')
			return true;
		if(input == 'n')
			return false;
		
		cout << "Error... invalid entry." << endl;
	}
}


int main()
{
	//random seeding
	srand(time(NULL));
	
	int wid = 5, len = 5, height = 5;
	bool debug = false;
	bool play = true;
	Game g;

	while(play){
		bool saveLoaded = g.is_save_loaded();

		if(!saveLoaded){
			g = Game();
			len = length_input();
			wid = width_input();
			height = height_input();
		}
		debug = debug_input();
	
		g.play_game(wid, len, height, debug);

		play = should_play_again();
		if(play)
			g.prompt_load_save();
	}

	return 0;
}