#include "Game.h"
#include <string>
#include <iostream>

Game::Game(Player* p1, Player* p2) {
    this->p1 = p1;
    this->p2 = p2;
    setup(); 
}
Game::~Game() {
    delete this;
}

void Game::setup() {
    this->pile = new LinkedList();
    pile->addFront(FIRST_PLAYER);
    this->currentPlayer = p1;
    generateFactories();
}

void Game::generateFactories() {
    for(int i=0; i<FACTORIES; ++i) {
        for(int j=0; j<FACTORY_SIZE; ++j) {
            this->factories[i][j] = randomTile();
        }
    }
}

Tile Game::randomTile() {
    Tile tileSelection[5] = {RED,YELLOW,BLUE,LIGHT_BLUE,BLACK};
    int ran = rand() % 5;
    return tileSelection[ran];
}

//-------------------GAMEPLAY LOOP LOGIC-----------------

void Game::play() {

    printFactories();
    bool gameEnd = false;
    while(!gameEnd){
        round();
    }
    std::cout << "Game over." << std::endl;
    
}

void Game::round() {

    bool roundEnd = false;
    while(!roundEnd) { 
        //print mosaic for the player whos turn it is
        printMosaic(this->currentPlayer);
        if(turn(this->currentPlayer)) {
            std::cout << "success" << std::endl;
        }
        else { 
            std::cout << "fail" << std::endl;
        }
        printMosaic(this->currentPlayer);
        std::cout << std::endl;
        switchPlayer(this->currentPlayer);
        printFactories();
        roundEnd = checkRoundEnd();
    }

    std::cout << "end of round" << std::endl;

}

bool Game::turn(Player* p) {

    bool output = true;
    std::string key;
    int factoryRow;
    Tile tile;
    int row;
    LinkedList* found = new LinkedList();
    
    std::cout << "it is " << p->getName() << "'s turn: " << std::endl;
    //take input string, split into args
    std::cin >> key >> factoryRow >> tile >> row;
    //for case insensitivity during comparison
    tile = toupper(tile);
    
    if(p->countStorage(row,tile) < 0) {
        output = false;
    } else { 
        if(key == "turn" || key == "TURN") {
            //if specified factory is standard (not pile)
            if(factoryRow>0 && factoryRow<=FACTORIES) {
                //add specified tiles to temporary "found" place before storage, others to pile
                for(int i=0; i<FACTORY_SIZE; ++i) {
                    if(this->factories[factoryRow-1][i] == tile && (row - p->countStorage(row,tile)) > 0) {
                        found->addFront(factories[factoryRow-1][i]);
                    }
                    //if specified storage row is full, overflow into broken tiles (floor)
                    else if (this->factories[factoryRow-1][i] == tile) {
                        p->addToBroken(this->factories[factoryRow-1][i]);
                    }
                    else {
                        this->pile->addBack(this->factories[factoryRow-1][i]);
                    }
                    this->factories[factoryRow-1][i] = NO_TILE;
                }
                if(found->size() == 0) {
                    output = false;
                } else {
                    //I think setStorage should be addToStorage instead - you can add tiles to non-empty storages, would be impossible to fill lower storages otherwise
                    p->setStorage(row,found);
                    output = true;
                }
            }
            //if middle pile factory is chosen
            else if(factoryRow == 0) {
                if(this->pile->get(0) == FIRST_PLAYER) {
                    p->addToBroken(FIRST_PLAYER);
                    this->pile->removeFront();
                }
                int counter = 0;
                for(int i=0; i - counter<this->pile->size(); ++i) {
                    int adjustedCount = i-counter;
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
                    output = false;
                }
                else {
                    p->setStorage(row,found);
                    output = true;
                }
            }
        }     
    }

    return output;

}

//--------------END OF GAMEPLAY LOOP LOGIC-----------------

void Game::printFactories() {

    std::cout << "Factories: " << std::endl;
    std::cout << "0: ";
    for(int j=0; j<this->pile->size(); ++j) {
        std::cout << pile->get(j) << " ";
    }
    std::cout << std::endl;
    for(int i=0; i<FACTORIES; ++i) {
        std::cout << i+1 << ": ";
        for(int j=0; j<FACTORY_SIZE; ++j) { 
            std::cout << this->factories[i][j] << " ";
        }
        std::cout << std::endl;
    }

}

void Game::printMosaic(Player* p) {

    std::cout << "Mosaic for " << p->getName() << ":" << std::endl;
    for(int i=0; i<SIZE; ++i) {
        std::cout << i+1 << ": ";
        for(int j=0; 4 > i + j; ++j) {
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

void Game::switchPlayer(Player* current) {

    if(current == p1) {
        this->currentPlayer = p2;
    }
    else {
        this->currentPlayer = p1;
    }

}

bool Game::checkRoundEnd() {

    bool noTilesLeft = true;

    if(this->pile->size() > 0) {
        noTilesLeft = false;
    } else {
        for(int i=0; noTilesLeft && i<5; ++i) {
            if(!(this->factories[i][0] == ' ')) {
                noTilesLeft = false;
            }
        }
    } 

    return noTilesLeft;

}
