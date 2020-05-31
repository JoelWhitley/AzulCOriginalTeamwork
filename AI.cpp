#include "AI.h"



AI::AI(Player* opponent, Player* controlled, Game* game) 
    : opponent(opponent), controlled(controlled),game(game) {
}

AI::~AI() {
}

int AI::makeTurn() {
    std::vector<std::tuple<int,Tile> > moves = game->availableTiles(controlled);
    bool foundValidLocation = false;
    int storageRow = 1; 

    //the tuple currently having it's data checked to see if there's a valid move
    auto currentTuple = moves[0];

    //tracks the index of the current tuple
    int tupleCounter = 0;
    
    while(!foundValidLocation) {
        Tile toCheck = std::get<1>(currentTuple);
        if(controlled->mosaicRowHasTile(storageRow,toCheck) == false && controlled->getStorageCell(storageRow - 1,0) == NO_TILE) {
            foundValidLocation = true;
        }
        else {
            storageRow++;
        }

        if(storageRow == FACTORIES) {
            ++tupleCounter;
            currentTuple = moves[tupleCounter];
        }
    }
    std::cout << std::get<0>(currentTuple) << ", " << std::get<1>(currentTuple) << ", " << storageRow << std::endl;
    this->game->turn(controlled,std::get<0>(currentTuple),std::get<1>(currentTuple),storageRow);
    return 0;
}