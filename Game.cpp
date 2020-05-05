#include "Game.h"
#include "LinkedList.h"
#include <string>
#include <iostream>

Game::Game(Player* p1, Player* p2) {
    this->p1 = *p1;
    this->p2 = *p2;
};
Game::~Game() {
    delete this;
}

char Game::factoryGeneration() {
    char tileSelection[5] = {RED,YELLOW,BLUE,LIGHT_BLUE,BLACK};
    int ran = rand() % 5;
    return tileSelection[ran];
}

void Game::play() {
    Player playersTurn = p1;

    //populate factories, to be replaced with randomiser
    char options[5] = {RED,YELLOW,BLUE,LIGHT_BLUE,BLACK};
    char factories[5][4];
    LinkedList* pile = new LinkedList;
    pile->addFront('F');
    std::cout << "Factories: " << std::endl;
    std::cout << "0: " << pile->get(0) << std::endl;
    for(int i = 0;i<5;++i) {
        std::cout << i + 1 << ": ";
        for(int j = 0;j<4;++j) {
            factories[i][j] = options[i];
            std::cout << factories[i][j] << " ";
        }
        std::cout << std::endl;
    }

    //print mosaic for the player whos turn it is
    printMosaic(&playersTurn);

}

void Game::printMosaic(Player* p) {
    std::cout << "Mosaic for " << p->getName() << ":" << std::endl;
    for(int i = 0;i < 5;++i) {
        std::cout << i + 1 << ": ";
        for(int j = 0;4 > i + j;++j) {
            std::cout << " ";
        }
        p->printStorageLine(i);
        std::cout << "||";
        p->printMosaicLine(i);
        std::cout << std::endl;
    }
    
}