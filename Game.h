#include "Player.h"


class Game {
    public:
    
        Game(Player* p1, Player* p2);
        ~Game();
        void setup();
        void generateFactories();
        Tile randomTile();

        void play();
        void round();
        bool turn(Player* p);

        void printMosaic(Player* p);
        void printFactories();
        void switchPlayer(Player* current);
        bool checkRoundEnd();
        void storageToMosaic();
        void moveTiles(Player* p);
        
    private:

        Player* p1;
        Player* p2;
        Player* currentPlayer;
        Tile factories[5][4];
        LinkedList* pile;
};