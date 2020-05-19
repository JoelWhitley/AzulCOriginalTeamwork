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
        int userInput(Player* p);
        void round();
        void endRound();
        bool turn(Player* p, int factory, Tile tile, int row);

        void printMosaic(Player* p);
        void printFactories();
        void switchPlayer();
        bool checkRoundEnd();
        bool checkGameEnd(Player* p);
        void storageToMosaic();
        void moveTiles(Player* p);
        int getMosaicColumnByTile(int row, Tile tile);
        int matchingTilesInFactory(int factory, Tile tile);
        void printHelp();
        void saveGame();
        void loadGame(std::string filename);
        
    private:

        Player* p1;
        Player* p2;
        Player* currentPlayer;
        Tile factories[FACTORIES][FACTORY_SIZE];
        LinkedList* pile;
        int roundNumber;
        Player* nextPlayer;
        Player* playerWithFPTile;
        bool gameEnd;
};
