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
        this->storage[i] = new Tile[i + 1];    
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

//returns the amount of tiles in input row that match input tile, returns -1 if there are different coloured tiles
int Player::countStorage(int row, Tile tile) {
    int count = 0;
    for(int i=0; count>=0 && i<row; ++i) {
        if(this->storage[row-1][i] != NO_TILE && this->storage[row-1][i] != tile) {
            count = -1;
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
    Tile tile = toInsert->get(0);
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

//counts the total amount of tiles chained to the input co-ordinate vertically and horizontally
int Player::calcScore(int row, int col){
    
    int score = 1;
    bool connected;
    
    //check row left 
    if(col>0){
        connected = true;
        for(int c=col-1; c>0; c--){   
            if(connected && mosaic[row][c] != NO_TILE){
                score++;
            }
            else{
                connected = false;
            }
        }
    }
    //check row right
    if(col<SIZE){
        connected = true;
        for(int c=col+1; c<SIZE; c++){   
            if(connected && mosaic[row][c] != NO_TILE){
                score++;
            }
            else{
                connected = false;
            }
        }
    }
    //check column up
    if(row>0){
        connected = true;
        for(int r=row-1; r>0; r--){   
            if(connected && mosaic[r][col] != NO_TILE){
                score++;
            }
            else{
                connected = false;
            }
        }
    }
    //check column down
    if(row<SIZE){
        connected = true;
        for(int r=row+1; r<SIZE; r++){   
            if(connected && mosaic[r][col] != NO_TILE){
                score++;
            }
            else{
                connected = false;
            }
        }
    }
    
    return score;
    
}

void Player::addToBroken(Tile tile) {
    this->broken->addFront(tile);
}

LinkedList* Player::getBroken() {
    return this->broken;
}