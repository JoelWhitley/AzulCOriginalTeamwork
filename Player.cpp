#include "Player.h"

Player::Player(std::string name)
   : name(name), points(0) {
}


Player::~Player() {
   delete this;
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