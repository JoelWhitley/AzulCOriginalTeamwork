#include "Game.h"
#include <string>
#include <iostream>

Game::Game(Player* p1, Player* p2) {
    this->p1 = p1;
    this->p2 = p2;
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
    this->currentPlayer = p1;
    this->pile = new LinkedList();
    pile->addFront('F');
    std::cout << "Factories: " << std::endl;
    std::cout << "0: " << pile->get(0) << std::endl;
    for(int i = 0;i<5;++i) {
        std::cout << i + 1 << ": ";
        for(int j = 0;j<4;++j) {
            this->factories[i][j] = factoryGeneration();
            std::cout << this->factories[i][j] << " ";
        }
        std::cout << std::endl;
    }

    //print mosaic for the player whos turn it is
    printMosaic(this->currentPlayer);
    if(turn(this->currentPlayer)) {
        std::cout << "success" << std::endl;
    }
    else { 
        std::cout << "fail" << std::endl;
    }
    printMosaic(this->currentPlayer);
    printFactories();

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
    std::cout << "Broken: ";
    for(int i =0;i < p->getBroken()->size();++i) {
        std::cout << p->getBroken()->get(i) << " ";
    }
    std::cout << std::endl;

    
}
bool Game::turn(Player* p) {
    std::cout << "it is " << p->getName() << "'s turn: " << std::endl;
    std::string key;
    int factoryRow;
    char tile;
    int row = 0;
    std::cin >> key >> factoryRow >> tile >> row;
    LinkedList* found = new LinkedList();
    if(p->countStorage(row,tile) < 0) {
        return false;
    }
    
    if(key == "turn") {
        if(factoryRow < 6 && factoryRow >= 0) {
            for(int i = 0;i<4;++i) {
                if(this->factories[factoryRow-1][i] == tile && row > i) {
                    found->addFront(factories[factoryRow-1][i]);
                }
                else if (this->factories[factoryRow-1][i] == tile) {
                    p->addToBroken(this->factories[factoryRow-1][i]);
                }
                else {
                    this->pile->addBack(this->factories[factoryRow-1][i]);
                }
                this->factories[factoryRow-1][i] = ' ';
            }
            if(found->size() == 0) {
                return false;
            }
            else {
                p->setStorage(row,found);
                return true;
            }
        }
    }
    return false;

}
void Game::printFactories() {
    std::cout << "Factories: " << std::endl;
    std::cout << "0: ";
    for(int j = 0;j<this->pile->size();++j) {
        std::cout << pile->get(j) << " ";
    }
    std::cout << std::endl;
    for(int i = 0;i<5;++i) {
        std::cout << i + 1 << ": ";
        for(int j = 0;j<4;++j) { 
            std::cout << this->factories[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
void Game::switchPlayer(Player* current) {
    if(current == p1) {
        this->currentPlayer = p2;
    }
    else {
        this->currentPlayer = p1;
    }
}