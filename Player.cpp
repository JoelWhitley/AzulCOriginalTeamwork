#include "Player.h"

Player::Player(std::string name)
   : name(name), points(0) {
}

Player::Player(std::string name, int points)
   : name(name), points(points) {
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
    points += points;
}