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

    //populate factories
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

    
}