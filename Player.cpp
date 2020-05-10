#include "Player.h"


Player::Player(std::string name)
   : name(name), points(0) {
       populateStorages();
       this->broken = new LinkedList();
}


Player::~Player() {
   
}

std::string Player::getName(){
    return name;
}

int Player::getPoints(){
    return points;
}

void Player::addPoints(int points){
    this->points += points;
}

void Player::populateStorages() {
    for(int i = 0;i < 5;++i) {
        this->storage[i] = new char[i + 1];    
        for(int j = 0;j < i + 1;++j) {
            this->storage[i][j] = NO_TILE; 
        }
    } 
    for(int i = 0;i < 5;++i) {   
        for(int j = 0;j < 5;++j) {
            this->mosaic[i][j] = NO_TILE;    
        }
    }    
}

// this method returns the amount of tiles of the same colour as the inputed one are in the row, returns -1 if there is different coloured tiles
int Player::countStorage(int row,char tile) {
    int count = 0;
    for(int i = 0;i < row;++i) {
        if(this->storage[row-1][i] != NO_TILE && this->storage[row-1][i] != tile) {
            return -1;
        }
        else {
            if(this->storage[row-1][i] == tile) {
                ++count;
            }
        }
    }
    return count;
}
void Player::setStorage(int row, LinkedList* toInsert) {
    char tile = toInsert->get(0);
    int count = this->countStorage(row,tile);

    for(int i = 0;i < toInsert->size();++i) {
        if(count <= row) {
            this->storage[row-1][count] = tile;
            ++count;
        }
        else {
            this->broken->addFront(tile);
            ++count;
        }
    }
}
void Player::printStorageLine(int row) {
    for(int i = row;i >= 0;--i) {
        std::cout << this->storage[row][i];        
    }   
}
void Player::printMosaicLine(int row) {
    for(int i = 0;i < 5;++i) {
        std::cout << this->mosaic[row][i];
    }
}
void Player::addToBroken(char tile) {
    this->broken->addFront(tile);
}
LinkedList* Player::getBroken() {
    return this->broken;
}
