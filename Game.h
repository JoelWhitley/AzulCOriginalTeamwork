#include "Player.h"


class Game {
    public:
        Game(Player* p1, Player* p2);
        ~Game();
        char factoryGeneration();
        void play();
        void printMosaic(Player* p);
        bool turn(Player* p);
        void printFactories();
        void switchPlayer(Player* current);
    private:
        Player* p1;
        Player* p2;
        Player* currentPlayer;
        char factories[5][4];
        LinkedList* pile;
};