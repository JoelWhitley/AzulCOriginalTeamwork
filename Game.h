#include "Player.h"

class Game {
    public:
        Game(Player* p1, Player* p2);
        ~Game();
        char factoryGeneration();
        void play();
        void printMosaic(Player* p);
    private:
        Player p1;
        Player p2;
};