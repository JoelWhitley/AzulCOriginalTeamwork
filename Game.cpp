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
    bool roundEnd = false;
    printMosaic(this->currentPlayer);
    while(!roundEnd) {       
        if(turn(this->currentPlayer)) {
            std::cout << "success" << std::endl;
            switchPlayer(this->currentPlayer);
        }
        else { 
            std::cout << "Fail, please try again" << std::endl;
        }    
        printFactories();
        printMosaic(this->currentPlayer); 
        roundEnd = checkRoundEnd();
    }
    moveTiles(p1);
    moveTiles(p2);
    printMosaic(p1);
    printMosaic(p2);
    std::cout << "end of round" << std::endl;

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
    bool failCondition = false;
    
    if(tile == 'F') {
        return false;
    }
    for(int i = 0;i<4;++i) {
        if(this->factories[factoryRow-1][i] == tile) {
            failCondition = true;
        }
    }
    if(failCondition == false) {
        return failCondition;
    }
    if(key == "turn") {
        if(row < 6 && row >= 0) {
            if(p->countStorage(row,tile) < 0) {
                return false;
            }
            if(factoryRow < 6 && factoryRow > 0) {
                for(int i = 0;i<4;++i) {
                    if(this->factories[factoryRow-1][i] == tile && (row - p->countStorage(row,tile)) > 0) {
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
            else if(factoryRow == 0) {
                if(this->pile->get(0) == 'F') {
                    p->addToBroken('F');
                    this->pile->removeFront();
                }
                int counter = 0;
                for(int i = 0;i - counter < this->pile->size();++i) {
                    int adjustedCount = i - counter;
                    if(this->pile->get(adjustedCount) == tile && (row - p->countStorage(row,tile)) > 0) {
                        found->addFront(this->pile->get(adjustedCount));
                        this->pile->removeNodeAtIndex(adjustedCount);
                        counter++;
                    }
                    else if(this->pile->get(adjustedCount) == tile) {
                        p->addToBroken(this->pile->get(adjustedCount));
                        this->pile->removeNodeAtIndex(adjustedCount);
                        counter++;
                    }
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
bool Game::checkRoundEnd() {
    if(this->pile->size() > 0) {
        return false;
    }
    for(int i =0;i < 5;++i) {
        if(!(this->factories[i][0] == ' ')) {
            return false;
        }
    }
    return true;
}
void Game::moveTiles(Player* p) {
    int coloumn = 0;
    for(int row = 0;row < 5;++row) {
        char tile = p->getTile(row);
        if(p->countStorage(row + 1,tile) == row+1 && tile != NO_TILE) {
            coloumn = p->moveToMosaic(row + 1,tile);
            // put scoring methods here
            p->clearStorageRow(row);
        }
    }
}
