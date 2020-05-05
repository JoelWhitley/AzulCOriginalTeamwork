#include "Player.h"

Player::Player(std::string name)
   : name(name), points(0) {
       populateStorages();
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
            std::cout << this->storage[i][j]; 
        }
        std::cout << std::endl;
    } 
    for(int i = 0;i < 5;++i) {   
        for(int j = 0;j < 5;++j) {
            this->mosaic[i][j] = NO_TILE; 
            std::cout << this->mosaic[i][j]; 
        }
        std::cout << std::endl;
    }    
}

void Player::printStorageLine(int row) {
    for(int i = 0;i < row + 1;++i) {
        std::cout << this->storage[row][i];        
    }   
}
void Player::printMosaicLine(int row) {
    for(int i = 0;i < 5;++i) {
        std::cout << this->mosaic[row][i];
    }
}