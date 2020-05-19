#include <iostream>
#include <string>
#include <fstream>

#include "Player.h"
#include "Game.h"

void mainMenu();
void newGame();
void printCredits();
void loadGame();

bool running;
std::string player1Name;
std::string player2Name;
int player1Score;
int player2Score;
std::string player1ScoreString = "";
std::string player2ScoreString = "";
std::string nextTurn;

int main() {

    running = true;

    while(running){
        mainMenu();
    }
    
    return EXIT_SUCCESS;

}

void mainMenu() {

	int choice = 0;
	
	std::cout << "*** Welcome to 🅰 🆉 🆄 🅻 ***" << std::endl;
	std::cout << "(1): New Game" << std::endl;
    std::cout << "(2): Load Game" << std::endl;
    std::cout << "(3): Credits" << std::endl;
    std::cout << "(3): Exit without Saving" << std::endl;
    std::cout << ">";
	std::cin >> choice;
    std::cin.clear();
    std::cin.ignore();
    	  
	if(choice == 1) {
        newGame();
    }  
    else if(choice == 2) {
		loadGame();
    }
    else if(choice == 3) {
		printCredits();
    }
    else if(choice == 4) {
        std::cout << "Goodbye." << std::endl;
        running = false;
    }
	else {
		std::cout << "Invalid choice." << std::endl;	
	}

    return;

}

void newGame() {

    std::string player1name;
    std::string player2name;
    Game* game;

    std::cout << "Enter a name for Player 1:\n>";
    getline(std::cin, player1name);
    std::cout << "Enter a name for Player 2:\n>";
    getline(std::cin, player2name);

    Player* player1 = new Player(player1name);
    Player* player2 = new Player(player2name);

    game = new Game(player1, player2, 0);
    std::cout << player1->getName() << ", " << player2->getName() << ", let's play AZUL!\n\n";
    game->setup();
    game->play();
    
}

void loadGame() {
    Game* game;
    std::string filename;

    Player* player1 = new Player("");
    Player* player2 = new Player("");

    game = new Game(player1, player2, 0);
    std::cin >> filename;
    game->loadGame(filename);
}

void printCredits() {

    std::cout << "🅰 🆉 🆄 🅻" << std::endl;
    std::cout << "Created by:" << std::endl
    << "s3687337 Joshua Monaghan-Landy" << std::endl
    << "s3787473 Joel Whitley" << std::endl
    << "s3658845 Wen Koay" << std::endl
    << "s3837218 Dinesh Premanath Amarakone Urulugastenne Mudiyanselage" << std::endl;

    return;

}
