#include "Game.h"

#include <string>
#include <iostream>

Game::Game(Player* p1, Player* p2) {
    this->p1 = p1;
    this->p2 = p2;
    this->setup(1); 
}
Game::~Game() {
    delete this;
}

void Game::setup(int roundNumber) {
    this->pile = new LinkedList();
    pile->addFront(FIRST_PLAYER);
    this->currentPlayer = p1;
    generateFactories();
    if(roundNumber > 1) {
        p1->getBroken()->clear();
        p2->getBroken()->clear();
    }
}

void Game::generateFactories() {
    for(int i=0; i<FACTORIES; ++i) {
        for(int j=0; j<FACTORY_SIZE; ++j) {
            this->factories[i][j] = randomTile();
        }
    }
}

Tile Game::randomTile() {
    
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
    int roundNumber = 1;
    bool roundEnd = false;
    bool gameEnd = false;
    while(!gameEnd) {
        while(!roundEnd) { 
            
            printFactories();
            //print mosaic for the player whos turn it is
            printMosaic(this->currentPlayer);
            if(turn(this->currentPlayer)) {
                std::cout << "Success! " << currentPlayer->getName() << "'s new board:"<< std::endl;
                printMosaic(this->currentPlayer); 
                switchPlayer(this->currentPlayer);
            }
            else { 
                std::cout << "Fail, please try again." << std::endl;
            }    
            roundEnd = checkRoundEnd();
        }
        std::cout << "---END OF ROUND " << roundNumber << "---" << std::endl;
        std::cout << "SCORES FOR " << p1->getName() << ":" << std::endl;
        moveTiles(p1); 
        std::cout << "---END OF ROUND " << roundNumber << "---" << std::endl;
        std::cout << "SCORES FOR " << p2->getName() << ":" << std::endl; 
        moveTiles(p2);
        printMosaic(p1);
        printMosaic(p2);
        if(this->checkGameEnd(p1) == true || this->checkGameEnd(p2) == true) {
            gameEnd = true;
            
        }
        else {
            roundNumber++;
            std::cout << "---STARTING ROUND " << roundNumber << "---" << std::endl;
            roundEnd = false;
            this->setup(roundNumber);
            this->generateFactories();
        }
    }
}

bool Game::turn(Player* p) {
    //TODO: prevent adding tiles to storage when that type of tile already exists in that row of the mosaic

    std::string key;
    int factory;
    Tile tile;
    int row;
    LinkedList* found = new LinkedList();
    bool isValid = true;
    
    std::cout << "it is " << p->getName() << "'s turn: " << std::endl;
    //take input string, split into args
    if(!(std::cin >> key >> factory >> tile >> row)) {
        isValid = false;
        std::cin.clear();
        //std::cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
    }
    
    //for case insensitivity during comparison
    tile = toupper(tile);
    
    if(key=="exit" || key=="EXIT"){
        //back to menu, save, or something
    }    
    else if((key=="turn" || key=="TURN") && isValid == true) {
        isValid = false;
        
        //amount of specified tile in specified factory
        int matchingTiles = matchingTilesInFactory(factory, tile);

        if((matchingTiles>0) && (tile!=FIRST_PLAYER) && (p->countStorage(row,tile) >= 0)){ 
            //if specified factory is orbiting/standard (not pile)
            if((factory>0) && (factory<=FACTORIES)){
                for(int i=0; i<FACTORY_SIZE; ++i){
                    //add specified tiles to temporary "found" place before storage, if there's room in storage row
                    if(this->factories[factory-1][i] == tile && (row - p->countStorage(row,tile)) > 0) {
                        found->addFront(factories[factory-1][i]);
                    }
                    //add excess to broken tiles
                    else if(this->factories[factory-1][i] == tile) {
                        p->getBroken()->addBack(factories[factory-1][i]);
                    }
                    //add others to pile
                    else {
                        this->pile->addBack(this->factories[factory-1][i]);
                    }
                    this->factories[factory-1][i] = NO_TILE;
                }
                //add matching "found" tiles to specified storage row
                p->setStorage(row,found);
                isValid = true;
            }
            //if specified factory is middle pile
            else if(factory == 0){
                //move first_player tile to player's floor (if it's there)
                if(this->pile->get(0) == FIRST_PLAYER){
                    p->addToBroken(FIRST_PLAYER);
                    this->pile->removeFront();
                }
                int counter = 0;
                //add specified tiles to "found"
                for(int i=0; i - counter < this->pile->size(); ++i){
                    int adjustedCount = i - counter;
                    if(this->pile->get(adjustedCount) == tile && (row - p->countStorage(row,tile)) > 0) {
                        found->addFront(this->pile->get(adjustedCount));
                        this->pile->removeNodeAtIndex(adjustedCount);
                        counter++;
                    }
                    else if(this->pile->get(adjustedCount) == tile) {
                        p->getBroken()->addBack(this->pile->get(adjustedCount));
                        this->pile->removeNodeAtIndex(adjustedCount);
                        counter++;
                    }
                    
                }
                p->setStorage(row, found);
                isValid = true;
            }
        }
    }
    return isValid;
}


//--------------END OF GAMEPLAY LOOP LOGIC-----------------

int Game::matchingTilesInFactory(int factory, Tile tile){

    int count = 0;
    if(factory==0){
        for(int i=0; i < this->pile->size(); ++i) {
            if(this->pile->get(i) == tile){
                count++;
            }
        }
    }
    else if((factory>0) && (factory<=FACTORIES)){
        for(int i=0; i<FACTORY_SIZE; i++) {
            if(factories[factory-1][i] == tile){
                count++;
            }
        }
        
    }
    return count;

}

void Game::printFactories() {

    std::cout << "Factories: " << std::endl;
    std::cout << "0: ";
    for(int j=0; j<this->pile->size(); ++j) {
        std::cout << pile->get(j) << " ";
    }
    std::cout << std::endl;
    for(int i=0; i<FACTORIES; ++i) {
        std::cout << i+1 << ": ";
        for(int j=0; j < FACTORY_SIZE; ++j) { 
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
    for(int i=0; i < p->getBroken()->size(); ++i) {
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
    } 
    else {
        for(int i=0; noTilesLeft && i<FACTORIES; ++i) {
            if(!(this->factories[i][0] == NO_TILE)) {
                noTilesLeft = false;
            }
        }
    } 

    return noTilesLeft;

}
bool Game::checkGameEnd(Player* p) {
    bool gameComplete = false;
    for(int i = 0;i< SIZE;++i) {
        if(p->checkComplete(i) == true) {
            gameComplete = true;
        }
    }
    return gameComplete;
}

void Game::moveTiles(Player* p) {
    for(int row=0; row<SIZE; ++row) {
        Tile tile = p->getTile(row);
        if(p->countStorage(row+1, tile) == row+1 && tile != NO_TILE) {
            p->moveToMosaic(row+1, tile);
            std::cout << "Row " << row+1 << ":" << std::endl;
            p->addPoints(p->calcScore(row, getMosaicColumnByTile(row, tile)));
            p->clearStorageRow(row);
        }
    }
    p->addPoints(demerits[p->getBroken()->size()]);
}

int Game::getMosaicColumnByTile(int row, Tile tile){
    
    int lineIndex = 0;
    for(int i=0; i<SIZE; ++i) {
        if(tile == topRowOrder[i]) {
            lineIndex = i;
        }
    }
    int output = (lineIndex+row) % SIZE;
    return output;

}

