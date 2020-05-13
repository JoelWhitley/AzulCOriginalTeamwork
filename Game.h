#include "Player.h"
#include "Rules.h"

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
        int getMosaicColumnByTile(int row, Tile tile);
        int matchingTilesInFactory(int factory, Tile tile);
        
    private:

        Player* p1;
        Player* p2;
        Player* currentPlayer;
        Tile factories[FACTORIES][FACTORY_SIZE];
        LinkedList* pile;
        int roundNumber;

};