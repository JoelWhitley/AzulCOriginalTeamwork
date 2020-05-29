#include "AI.h"


AI::AI(Player* opponent, Player* controlled, Game* game) 
    : opponent(opponent), controlled(controlled),game(game) {
}

AI::~AI() {
}

int AI::makeTurn() {
    this->game->turn(controlled,3,LIGHT_BLUE,4);
    return 0;
}