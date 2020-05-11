#include <iostream>
#include <string>
#include "Player.h"
#include "Game.h"

void mainMenu();
void newGame();
void printCredits();

bool running;
std::string player1Name;
std::string player2Name;
int player1Score;
int player2Score;

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
    std::cout << "(2): Credits" << std::endl;
    std::cout << "(3): Exit without Saving" << std::endl;
	std::cin >> choice;
    	
	switch(choice)
	{
	case 1:
        newGame();
        break;
    case 2:
        loadGame();
    case 3:
		printCredits();
		break;
    case 4:
        running = false;
        std::cout << "Goodbye." << std::endl;
        break;
	default:
		std::cout << "Invalid choice." << std::endl;
		break;
	}

    return;

}

void newGame() {

    std::string player1name;
    std::string player2name;
    // Game game;

    std::cout << "Enter a name for Player 1:" << std::endl;
    std::cin >> player1name;
    std::cout << "Enter a name for Player 2:" << std::endl;
    std::cin >> player2name;

    Player* player1 = new Player(player1name);
    Player* player2 = new Player(player2name);

    Game* game = new Game(player1, player2);
    std::cout << player1->getName() << ", " << player2->getName() << ", let's play AZUL!" << std::endl;
    game->play();
    
}

void printCredits() {

    std::cout << "🅰 🆉 🆄 🅻" << std::endl;
    std::cout << "Created by:" << std::endl
    << "s3687337 Joshua Monaghan-Landy" << std::endl
    << "s3787473 Joel Whitley" << std::endl
    << "s3837218 Dinesh Premanath Amarakone Urulugastenne Mudiyanselage" << std::endl 
    << "s3658845 Wen Koay" << std::endl;

    return;

}

void loadGame(){
    std::string filename;
    std::cout << "Enter the filename from which to load a game\n> ";
    std::cin >> filename;
    std::ifstream file;
    file.open(filename);
    if(!file)
    {
        std::cout << "File does not exist";
        menu();
        return;
    }
    else //So if the file was found
    {
        std::cout << "AZUL game successfully loaded\n\n";
        //Should read in the player name , score , board and empty spaces.
        //The values in here are all examples
        player1Name = "";
        player1Score = 0;
      
        //Example values too, really they'd be reading lines from the file
        player2Name = "";
        player2Score = 0;

        file.close();
        return;
    }
    file.close();
    menu();
    return;
}
