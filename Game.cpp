#include "Game.h"

Game::Game(Player* p1, Player* p2, int seed) {
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
    this->tileBag = new LinkedList();
    generateTileBag(0);
    this->boxLid = new LinkedList();
    this->currentPlayer = playerWithFPTile;    
    generateFactories();   
    saved=true; 
}

void Game::generateTileBag(int seed){

    if(seed==0){
        //TODO: randomly distributed
        //currently sequential
        for(int tile=0; tile<GAME_TILES; tile++){
            tileBag->addBack(tileSelection[tile%SIZE]);
        }
    }
    else {
        //specific seed generation
    }

}

void Game::generateFactories() {
    for(int i=0; i<FACTORIES; ++i) {
        for(int j=0; j<FACTORY_SIZE; ++j) {
            this->factories[i][j] = getTileFromBag();
        }
    }
}

Tile Game::getTileFromBag(){    
    if(tileBag->getSize()==0){
        emptyLidIntoBag();
    }
    if(tileBag->getSize()>0){
        Tile first = tileBag->get(0);
        tileBag->removeFront();
        return first;
    }
    else {
        return NO_TILE;
    }
}

void Game::emptyLidIntoBag(){
    Tile currTile;
    for(int i=0; i<boxLid->getSize(); i++){
        currTile = boxLid->get(0);
        tileBag->addBack(currTile);
        boxLid->removeFront();
    }
}

Tile Game::randomTile() {    
    int ran = rand() % SIZE;
    return tileSelection[ran];
}

//-------------------GAMEPLAY LOOP LOGIC-----------------

void Game::play() {
    gameEnd = false;
    while(!gameEnd){
        round();
    }
    std::cout << "Game over." << std::endl;    
}

void Game::round() {

    bool exit = false;
    bool roundEnd = false;
    if(!resumed){
        generateFactories();
    }
    resumed = false;

    std::cout << "\n---FACTORY OFFER PHASE---\n" << std::endl;
    
    while(!roundEnd) {         
        printFactories();
        std::cout << std::endl;
        std::cout << this->currentPlayer->getName() << "'s board:" << std::endl;
        printBoard(this->currentPlayer);

        int outcome = turnPrompt(this->currentPlayer);       
        if(outcome == OUTCOME_TURNSUCCESS) {
            std::cout << "\nSuccess! " << this->currentPlayer->getName() << "'s new board:" << std::endl;
            printBoard(this->currentPlayer); 
            std::cout << std::endl;
            switchPlayer();
        }
        else if(outcome == OUTCOME_TURNFAIL){ 
            std::cout << "Turn failed, please try something else.\n" << std::endl;
        }  
        else if(outcome == OUTCOME_EXIT){
            exit = true;
            roundEnd = true;
        }
        if(!roundEnd){
            roundEnd = checkRoundEnd();
        }          
    }

    if(exit){
        gameEnd = true;
    }
    else {
        std::cout << "---FACTORIES DEPLETED!---" << std::endl;
        std::cout << "---WALL TILING PHASE---" << std::endl;
        std::cout << "SCORES FOR " << p1->getName() << ":" << std::endl;
        moveTiles(p1); 
        std::cout << "SCORES FOR " << p2->getName() << ":" << std::endl; 
        moveTiles(p2);
        std::cout << p1->getName() << "'s board:" << std::endl;
        printBoard(p1);
        std::cout << p2->getName() << "'s board:" << std::endl;
        printBoard(p2);
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
        endGame();
        gameEnd = true;        
    }  

}

void Game::endGame(){

    std::cout << "!!! ROW COMPLETED !!!\n";
    if(!isTie){
        winner = p1->getPoints()>p2->getPoints()?p1:p2;
        std::cout << "!!! " << winner->getName() << " wins with " << winner->getPoints() << " points !!!" << std::endl;
    }    
    else{
        std::cout << "!!! GAME TIED !!!";
    }

}

bool Game::turn(Player* p, int factory, Tile tile, int row) {

    saved = false;
    //TODO: prevent adding tiles to storage when that type of tile already exists in that row of the mosaic

    std::vector<Tile> found;
    bool isValid = false;
    bool compatibleTileRow = false;
    int roomInRow;
    bool validRow = true;

    //amount of specified tile in specified factory
    int matchingTiles = matchingTilesInFactory(factory, tile);
    
    if(row != FLOOR_ROW){
        compatibleTileRow = p->countStorage(row,tile) >= 0;
        roomInRow = row-p->countStorage(row,tile);  
        validRow = compatibleTileRow && roomInRow>0 && !p->mosaicRowHasTile(row, tile);
    }    

    if((matchingTiles>0) && (tile!=FIRST_PLAYER) && validRow){ 
        //if specified factory is orbiting/standard (not middle pile)
        if((factory>0) && (factory<=FACTORIES)){ 
            for(int i=0; i<FACTORY_SIZE; ++i){
                //if current tile is the player's chosen one
                if(this->factories[factory-1][i] == tile){
                    if(row==FLOOR_ROW){ 
                        found.push_back(factories[factory-1][i]);
                    }
                    //prepare specified tiles to be sent to storage (if there's room)
                    else if(roomInRow > 0) {
                        found.push_back(factories[factory-1][i]);
                        roomInRow--;
                    }
                    //add excess to broken tiles
                    else { 
                        p->getBroken()->addBack(factories[factory-1][i]);
                    }
                }                
                //add other tiletypes to pile         
                else {  
                    this->pile->addBack(this->factories[factory-1][i]);
                }
                this->factories[factory-1][i] = NO_TILE;
            }
            isValid = true;
        }
        //if specified factory is middle pile
        else if(factory == 0){
            int counter = 0;
            //move first_player tile to player's floor (if it's there)
            if(this->pile->get(0) == FIRST_PLAYER){
                p->addToBroken(FIRST_PLAYER);
                this->pile->removeFront();
                counter++;
            }
            //add specified tiles to "found"
            for(int i=counter; i-counter < this->pile->getSize(); ++i){
                int adjustedCount = i-counter;
                if(this->pile->get(adjustedCount) == tile){
                    if(row==FLOOR_ROW){                    
                        found.push_back(this->pile->get(adjustedCount));
                        this->pile->removeNodeAtIndex(adjustedCount);
                        counter++;
                    }
                    //if there's still room in specified row, send it there
                    else if(roomInRow > 0) {
                        found.push_back(this->pile->get(adjustedCount));
                        this->pile->removeNodeAtIndex(adjustedCount);
                        roomInRow--;
                        counter++;
                    }
                    //if not, send to floor
                    else {
                        p->getBroken()->addBack(this->pile->get(adjustedCount));
                        this->pile->removeNodeAtIndex(adjustedCount);
                        counter++;
                    }
                }
                
                
            }
            isValid = true;
        }
        if(isValid){
            p->addToStorage(row,found,boxLid);
        }    
    }
    return isValid;

}

//--------------END OF GAMEPLAY LOOP LOGIC-----------------

int Game::turnPrompt(Player* p){

    int outcome = -1;
    std::string input;
    std::string command;
    int factory;
    Tile tile;
    int row;
    
    std::cout << "it is " << p->getName() << "'s turn: \n";

    //take input string, split into args
    input = userInput();
    std::stringstream(input) >> command >> factory >> tile >> row;
    std::stringstream(input).clear();
    std::stringstream(input).ignore();
       
    if(command=="exit" || command=="EXIT"){
        outcome = OUTCOME_EXIT;
        if(!saved){
            std::cout << "Would you like to save? (y/n):\n";
            std::string decision = userInput();
            while(decision != "Y" && decision != "y" && decision != "N" && decision != "n"){
                std::cout << "Please try again.\n";
                decision = userInput();
            }
            if(decision=="Y" || decision=="y"){
                saveGame();
            }
        }
    }
    else if(command=="save" || command=="SAVE"){
        saveGame();
        outcome = OUTCOME_SAVE;        
    }
    else if(command=="turn" || command=="TURN"){
        //for case insensitivity during later comparison
        tile = toupper(tile);
        outcome = turn(p, factory, tile, row) ? OUTCOME_TURNSUCCESS : OUTCOME_TURNFAIL;
    }
    else if(command=="help" || command=="HELP"){
        printHelp();
        outcome = OUTCOME_INVALID;
    }
    else{
        outcome = OUTCOME_INVALID;
        std::cout << "Invalid input. Type \"help\" to see list of commands.\n";
    }

    return outcome;

}

std::string Game::userInput(){

    std::string input;
    std::cout << "> "; 
    getline(std::cin, input);
    if(std::cin.eof()){
        std::cout << "Goodbye." << std::endl;
        exit(EXIT_SUCCESS);
    }  
    std::cin.clear();

    return input;

}

int Game::matchingTilesInFactory(int factory, Tile tile){

    int count = 0;
    if(factory==0){
        for(int i=0; i < this->pile->getSize(); ++i) {
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
    for(int j=0; j<this->pile->getSize(); ++j) {
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

void Game::printBoard(Player* p) {

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
    for(int i=0; i < p->getBroken()->getSize(); ++i) {
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
    if(this->pile->getSize() > 0) {
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
    p->addPoints(demerits[p->getBroken()->getSize()]);
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

void Game::printHelp(){
    std::cout << "------VALID COMMANDS------\n" <<
    "turn <factory> <tile> <destination row (" << FLOOR_ROW << " for the floor)>\n" <<
    "save\n" <<
    "exit\n" <<
    "--------------------------\n";
}

void Game::saveGame(){

    std::string fileName;
    std::cout << "\nEnter a name for the save file (or leave blank to cancel):";
    fileName = userInput();
    if(fileName.empty()){
        std::cout << "Save aborted." << std::endl;
    }
    else { 

        Player* players[] = {this->p1,this->p2};

        std::ofstream file;
        file.open(fileName);

        file << p1->getName()<< std::endl;                  //PLAYER 1 NAME
        file << p2->getName()<< std::endl;                  //PLAYER 2 NAME
        file << p1->getPoints() << std::endl;               //PLAYER 1 POINTS
        file << p2->getPoints() << std::endl;               //PLAYER 2 POINTS
        file << currentPlayer->getName() << std::endl;      //NEXT TURN

        for(int j=0; j<this->pile->getSize(); ++j){         //FACTORY 0 (PILE)
            file << pile->get(j) << " ";
        }        
        file << std::endl;

        for(int i=0; i<FACTORIES; ++i){                     //FACTORIES
            for(int j=0; j < FACTORY_SIZE; ++j){
                file << this->factories[i][j] << " ";
            }
            file << std::endl;
        }
        for(Player* p : players){
            for(int i=0; i<SIZE; ++i) {                     //PLAYER MOSAIC ROWS
                for(int j=0; j<SIZE; ++j) {
                    file << p->mosaic[i][j] << " ";
                }
                file << std::endl;
            }
        }
        for(Player* p : players){
            for(int j=0; j<SIZE; ++j) {                     //PLAYER STORAGE ROWS
                for(int i=j; i>=0; --i) {
                    file << p->storage[j][i] << " ";
                }
                file << std::endl;
            }
        }
        for(Player* p : players){
            for(int i=0; i < FLOOR_SIZE; ++i) {             //PLAYER BROKEN TILES
                if(i<p->getBroken()->getSize()){
                    file << p->getBroken()->get(i) << " ";
                }
                else {
                    file << NO_TILE << " ";
                }            
            }    
            file << std::endl;   
        }            
        for(int i=0; i<boxLid->getSize(); i++){             //BOX LID TILES
            file << boxLid->get(i) << " ";
        }
        file << std::endl;
        for(int i=0; i<tileBag->getSize(); i++){            //BAG TILES            
            file << tileBag->get(i) << " ";
        }                       
        file << std::endl;
        file << "0";                                        //RANDOM SEED (unimplemented)              
                                                                                      
        saved = true;
        std::cout << "\nGame successfully saved.\n\n";
        file.close();
    }

}

void Game::loadGame(std::istream& inputStream) {      
    
    
    bool loadComplete = false;

    Player* players[] = {this->p1,this->p2};

    while(inputStream.good() && !loadComplete){
        
        std::string name; 
        for(Player* player:players) {                       //SET PLAYER NAMES                   
            inputStream >> name;                                
            player->setName(name);
        }

        int points; 
        for(Player* player:players) {                       //SET PLAYER POINTS                  
            inputStream >> points;                                
            player->setPoints(points);
        }

        std::string currentPlayer;
        inputStream >> currentPlayer;                       //NEXT TURN
        if(currentPlayer == name) {
            this->currentPlayer = p2;
        }
        else {
            this->currentPlayer = p1;
        }
        
        inputStream.ignore();        
        this->pile = new LinkedList;                        //FACTORY 0 (PILE)
        std::string pileLine;
        getline(inputStream, pileLine);
        Tile pileTile;
        std::stringstream ss(pileLine);
        while(ss >> pileTile){
            this->pile->addBack(pileTile);
        } 

        Tile factoryTile;                                   //FACTORIES
        for(int i=0; i<FACTORIES; ++i) {
            for(int j=0; j<FACTORY_SIZE; ++j) {
                inputStream >> factoryTile;
                this->factories[i][j] = factoryTile;
            }
        }

        Tile mosaicTile;                                    
        for(Player* player:players) {                       //SET MOSAIC ROWS
            for(int i=1; i<SIZE+1; ++i) {                     
                for(int j=0; j<SIZE; ++j) {
                    inputStream >> mosaicTile;
                    if(isupper(mosaicTile)) {
                        player->moveToMosaic(i,mosaicTile);
                    }
                    else {
                        player->moveToMosaic(i,NO_TILE);
                    }
                }
            }
        }
        
        LinkedList* toInsert = new LinkedList;              //SET STORAGE ROWS
        char storageTile;
        for(Player* player:players) {
            for(int i=1; i<SIZE+1; ++i) {
                for(int j=0; j<i; ++j) {
                    inputStream >> storageTile;
                    if(storageTile != NO_TILE){
                        toInsert->addFront(storageTile);
                    }                
                }
                if(toInsert->getSize()>0){
                    player->addToStorage(i,toInsert,boxLid);
                    toInsert->clear(); 
                }
            }
        }
    
        Tile brokenTile;
        for(Player* player:players) {                       //SET BROKEN TILES
            for(int i=0; i<FLOOR_SIZE; i++) {                   
                inputStream >> brokenTile;
                if(brokenTile != NO_TILE){
                    if(brokenTile == FIRST_PLAYER){
                        playerWithFPTile = player;
                    }
                    player->getBroken()->addBack(brokenTile);
                }            
            }
        }

        inputStream.ignore();  
        this->boxLid = new LinkedList;                      //BOX LID TILES
        std::string boxLidLine;
        getline(inputStream, boxLidLine);
        Tile boxLidTile;
        std::stringstream boxLidStream(boxLidLine);
        while(boxLidStream >> boxLidTile){
            this->boxLid->addBack(boxLidTile);
        } 

        inputStream.ignore();  
        this->tileBag = new LinkedList;                     //BAG TILES
        std::string tileBagLine;
        getline(inputStream, tileBagLine);
        Tile tileBagTile;
        std::stringstream tileBagStream(tileBagLine);
        while(tileBagStream >> tileBagTile){
            this->tileBag->addBack(tileBagTile);
        } 

        int randomSeed;                                     //RANDOM SEED
        inputStream >> randomSeed;

        loadComplete = true;

    }
    std::cout << "Game successfully loaded.\n\n";
    this->saved = true;
    this->resumed = true;

}