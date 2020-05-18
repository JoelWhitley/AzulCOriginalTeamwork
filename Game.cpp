#include "Game.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

Game::Game(Player* p1, Player* p2) {
    this->p1 = p1;
    this->p2 = p2;
    playerWithFPTile = p1;    
}
Game::~Game() {
    delete this;
}

void Game::setup() {
    this->pile = new LinkedList();
    pile->addFront(FIRST_PLAYER);
    this->currentPlayer = playerWithFPTile;
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
    int ran = rand() % SIZE;
    return tileSelection[ran];
}

//-------------------GAMEPLAY LOOP LOGIC-----------------

void Game::play() {
    this->setup();
    gameEnd = false;
    while(!gameEnd){
        round();
    }
    std::cout << "Game over." << std::endl;    
}

void Game::round() {

    bool exit = false;
    bool roundEnd = false;
    generateFactories();

    std::cout << "---FACTORY OFFER PHASE---" << std::endl;
    
    while(!roundEnd) {         
        printFactories();
        printMosaic(this->currentPlayer);

        int outcome = userInput(this->currentPlayer);       
        if(outcome == OUTCOME_TURNSUCCESS) {
            std::cout << "Success!" << std::endl;
            printMosaic(this->currentPlayer); 
            switchPlayer();
        }
        else if(outcome == OUTCOME_TURNFAIL){ 
            std::cout << "Turn failed, please try something else." << std::endl;
        }  
        else if(outcome == OUTCOME_EXIT){
            exit = true;
            roundEnd = true;
        }
        else if(outcome == OUTCOME_INVALID){
            std::cout << "Invalid entry." << std::endl;
        }
        if(!roundEnd){
            roundEnd = checkRoundEnd();
        }          
    }

    if(exit){
        gameEnd = true;
    }
    else {
        std::cout << "---TILES DEPLETED!---" << std::endl;
        std::cout << "--- WALL TILING PHASE---" << std::endl;
        std::cout << "SCORES FOR " << p1->getName() << ":" << std::endl;
        moveTiles(p1); 
        std::cout << "SCORES FOR " << p2->getName() << ":" << std::endl; 
        moveTiles(p2);
        printMosaic(p1);
        printMosaic(p2);
        endRound(); 
    }

}

void Game::endRound(){

    //at the end of the round, one of the two players has to have the FP tile.
    if(p1->getBroken()->contains(FIRST_PLAYER)){
        playerWithFPTile = p1;
    }
    else {
        playerWithFPTile = p2;
    }    
    p1->getBroken()->clear();
    p2->getBroken()->clear();

    std::cout << "---END OF ROUND---" << std::endl;

    if(this->checkGameEnd(p1) == true || this->checkGameEnd(p2) == true){
        gameEnd = true;        
    }  

}

int Game::userInput(Player* p){

    int outcome = -1;
    
    std::string input;
    std::string command;
    int factory;
    Tile tile;
    int row;
    
    std::cout << "it is " << p->getName() << "'s turn: " << std::endl;

    //take input string, split into args
    getline(std::cin, input);
    std::stringstream(input) >> command >> factory >> tile >> row;
    std::stringstream(input).clear();
    std::stringstream(input).ignore();
       
    if(command=="exit" || command=="EXIT"){
        outcome = OUTCOME_EXIT;
    }
    else if(command=="save" || command=="SAVE"){
        saveGame();
        outcome = OUTCOME_SAVE;
    }
    else if(command=="turn" || command=="TURN"){
        //for case insensitivity during later comparison
        tile = toupper(tile);
        std::cout << input << std::endl;
        outcome = turn(p, factory, tile, row) ? OUTCOME_TURNSUCCESS : OUTCOME_TURNFAIL;
    }
    else{
        outcome = OUTCOME_INVALID;
    }

    return outcome;

}

bool Game::turn(Player* p, int factory, Tile tile, int row) {
    //TODO: prevent adding tiles to storage when that type of tile already exists in that row of the mosaic

    LinkedList* found = new LinkedList();
    bool isValid = false;

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
        for(int j=0; FACTORY_SIZE > i + j; ++j) {
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

void Game::switchPlayer() {
    if(currentPlayer == p1) {
        this->currentPlayer = p2;
    }
    else {
        this->currentPlayer = p1;
    }
}

bool Game::checkRoundEnd() {
    //initialised to true, as it's faster to prove that there -are- tiles left, than it is to prove there aren't
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
    for(int i=0; !gameComplete && i<SIZE; ++i) {
        if(p->checkComplete(i) == true) {
            gameComplete = true;
        }
    }
    return gameComplete;
}

//Sends tiles from player's FULL storage rows to their corresponding cells of the mosaic.
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

//The standard Azul board staggers the tile order of each row by 1. Returns the column matching the input row and tile, based on the global "firstRowOrder".
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

void Game::saveGame(){

    std::string fileName;
    std::cout << "\nEnter a name for the save file (or leave blank to cancel):\n";
    getline(std::cin, fileName);
    if(fileName.empty()){
        std::cout << "Save aborted." << std::endl;
    }
    else { 
        std::ofstream file;
        file.open(fileName);

        file << p1->getName()<< std::endl;                  //PLAYER 1 NAME
        file << p2->getName()<< std::endl;                  //PLAYER 2 NAME
        file << p1->getPoints() << std::endl;               //PLAYER 1 POINTS
        file << p2->getPoints() << std::endl;               //PLAYER 2 POINTS
        file << currentPlayer->getName() << std::endl;      //NEXT TURN

        for(int j=0; j<this->pile->size(); ++j){            //FACTORY 0
            file << pile->get(j) << " ";
        }
        
        file << std::endl;

        for(int i=0; i<FACTORIES; ++i){                     //FACTORY 1...FACTORIES
            for(int j=0; j < FACTORY_SIZE; ++j){
                file << this->factories[i][j] << " ";
            }
            file << std::endl;
        }

        for(int i=0; i<SIZE; ++i) {                         //PLAYER 1 MOSAIC ROW 1...SIZE
            for(int j=0; j<SIZE; ++j) {
                file << p1->mosaic[i][j];
            }
            file << std::endl;
        }

        for(int i=0; i<SIZE; ++i) {                         //PLAYER 2 MOSAIC ROW 1...SIZE
            for(int j=0; j<SIZE; ++j) {
                file << p2->mosaic[i][j];
            }
            file << std::endl;
        }

        for(int j=0; j<SIZE; ++j) {                         //PLAYER 1 STORAGE ROW 1...SIZE
            for(int i=j; i>=0; --i) {
                file << p1->storage[j][i];
            }
            file << std::endl;
        }

        for(int j=0; j<SIZE; ++j) {                         //PLAYER 2 STORAGE ROW 1...SIZE
            for(int i=j; i>=0; --i) {
                file << p2->storage[j][i];
            }
            file << std::endl;
        }

        for(int i=0; i < p1->getBroken()->size(); ++i) {    //PLAYER 1 BROKEN TILES
            file << p1->getBroken()->get(i) << " ";
        }

        for(int i=0; i < p2->getBroken()->size(); ++i) {    //PLAYER 2 BROKEN TILES
            file << p2->getBroken()->get(i) << " ";
        }

                                                            //BOX LID TILES
                                                            //BAG TILES
                                                            //RANDOM SEED
                                                
        std::cout << "\n\nGame successfully saved.\n> ";
        file.close();
    }

}

void Game::loadGame(std::string filename) {

}
