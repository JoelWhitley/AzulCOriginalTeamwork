#include "Player.h"


Player::Player(std::string name)
   : name(name), points(0), broken(new LinkedList()) {
       initialiseBoard();
}

Player::~Player() {
   
}

std::string Player::getName(){
    return name;
}

int Player::getPoints(){
    return points;
}

//adds points to player score (may subtract thanks to floor demerits, but score may not drop below zero)
void Player::addPoints(int points){

    this->points += points;
    if(this->points<0){
        this->points = 0;
    }
    if(points>0){
        std::cout << name << " scored " << points << " points, for a total of " << this->points << "." << std::endl;
    }
    if(points<0){
        std::cout << "Oops! Due to penalties, " << name << " lost " << abs(points) << " points, bringing total down to " << this->points << "." << std::endl;
    }
}

void Player::initialiseBoard() {
    for(int i=0; i<SIZE; ++i) {
        this->storage[i] = new Tile[i+1];    
        for(int j=0; j<i+1; ++j) {
            this->storage[i][j] = NO_TILE; 
        }
    } 
    for(int i=0; i<SIZE; ++i) {   
        for(int j=0; j<SIZE; ++j) {
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
    for(int i=0; i<toInsert->size(); ++i) {
        if(count < row) {
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
    for(int i=row; i>=0; --i) {
        std::cout << this->storage[row][i];        
    }   
}

void Player::printMosaicLine(int row) {
    for(int i=0; i<SIZE; ++i) {
        std::cout << this->mosaic[row][i];
    }
}

//counts the total amount of tiles chained to the input co-ordinate vertically and horizontally
int Player::calcScore(int row, int col){
    
    int score = 0;
    bool connected = false;
    
    //-------row check--------
    int rowCount = 0;
    //check row left 
    if(col>=0){
        connected = true;
        for(int c=col-1; c>=0; c--){   
            if(connected && mosaic[row][c] != NO_TILE){
                rowCount++;
            }
            else{
                connected = false;
            }
        }
    }
    //check row right
    if(col<SIZE){
        connected = true;
        for(int c=col; c<SIZE; c++){   
            if(connected && mosaic[row][c] != NO_TILE){
                rowCount++;
            }
            else{
                connected = false;
            }
        }
    }
    if(rowCount>1){
        score += rowCount;
    }
    //------end row check-------
    //---------col check--------
    int colCount = 0;
    //check column up
    if(row>=0){
        connected = true;
        for(int r=row-1; r>=0; r--){   
            if(connected && mosaic[r][col] != NO_TILE){
                colCount++;
            }
            else{
                connected = false;
            }
        }
    }
    //check column down
    if(row<SIZE){
        connected = true;
        for(int r=row; r<SIZE; r++){   
            if(connected && mosaic[r][col] != NO_TILE){
                colCount++;
            }
            else{
                connected = false;
            }
        }
    }
    if(colCount>1){
        score += colCount;
    }
    //------end col check-------

    //if it's a lone tile, still include the point for placing it
    if(score==0){
        score++;
    }
    return score;
    
}

void Player::addToBroken(Tile tile) {
//TODO: add an if-not-full check first to prevent tiles being exiled
    this->broken->addFront(tile);
}

LinkedList* Player::getBroken() {
    return this->broken;
}

Tile Player::getTile(int row) {
    return storage[row][row];
}

void Player::clearStorageRow(int row) {
    for(int i = 0;i<row +1;++i) {
        this->storage[row][i] = NO_TILE;
    }
}

void Player::moveToMosaic(int row, Tile tile) {
    int lineIndex = 0;
    for(int i=0; i<SIZE; ++i) {
        if(tile == topRowOrder[i]) {
            lineIndex = i-1;
        }
    }
    lineIndex = (lineIndex + row) % SIZE;
    this->mosaic[row-1][lineIndex] = tile;
}

bool Player::checkComplete(int row) {
    bool completeRow = true;
    for(int i = 0;i< SIZE;++i) {
        if(this->mosaic[row][i] == NO_TILE) {
            completeRow = false;
        }
    }
    return completeRow;
}