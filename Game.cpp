#include "Game.h"
#include "SaveAndLoad.h"

Game::Game(Player* p1, Player* p2, int seed) : p1(p1), p2(p2) {
    pile = new LinkedList();
    tileBag = new LinkedList();
    boxLid = new LinkedList();
    playerWithFPTile = p1;
    currentPlayer = playerWithFPTile;
}
Game::~Game() {
    delete this;
}

void Game::setupRound() {    
    pile->addFront(FIRST_PLAYER);   
    this->currentPlayer = playerWithFPTile;    
    generateFactories();
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
    saved=true;
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
        setupRound();
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

    //empty broken tiles into boxLid, make note of who has the FP tile for next round
    //while there, check for endgame condition in each players' mosaic
    Player* players[] = {p1, p2};
    for(Player* player : players){
        for(int i=0; i<player->getBroken()->getSize(); i++){
            if(player->getBroken()->get(i)==FIRST_PLAYER){
                playerWithFPTile = player;
                player->getBroken()->removeNodeAtIndex(i);
            }
            else {
                boxLid->addBack(player->getBroken()->get(i));
            }
        }
        player->getBroken()->clear();
        gameEnd = checkGameEnd(player);
    }

    std::cout << "---END OF ROUND---" << std::endl;

    if(gameEnd){
        endGame();     
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
    int roomInRow = 0;
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

void Game::saveGame() {            
        SaveAndLoad* save = new SaveAndLoad(this,this->p1, this->p2, this->pile,
                this->tileBag, this->boxLid, this->playerWithFPTile, this->currentPlayer);
        save->saveGame();                                                                             
        saved = true;

}

void Game::loadGame(std::istream& inputStream) {      
    
    bool loadComplete = false;

    SaveAndLoad* load = new SaveAndLoad(this,this->p1, this->p2, this->pile,
                this->tileBag, this->boxLid, this->playerWithFPTile, this->currentPlayer);

    while(inputStream.good() && !loadComplete) {
        load->loadGame(inputStream);
    }
    this->saved = true;
    this->resumed = true;

}

void Game::setFactory(int row, int column, Tile insert) {
    this->factories[row][column] = insert;
}
Tile Game::getTileWithinFactory(int row, int column) {
    return this->factories[row][column];
}
