#ifndef AZUL_GAME
#define AZUL_GAME

#include "Player.h"
#include "Rules.h"
#include "LinkedList.h"

class Game {
    
    public:
    
        Game(Player* p1, Player* p2, int seed);
        ~Game();
        void setup();
        void generateFactories();
        void generateTileBag(int seed);
        Tile getTileFromBag();
        void emptyLidIntoBag();
        Tile randomTile();

        void play();
        int turnPrompt(Player* p);
        std::string userInput();
        void round();
        void endRound();
        void endGame();
        bool turn(Player* p, int factory, Tile tile, int row);

        void printBoard(Player* p);
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
        void loadGame(std::istream& filename);
        
    private:

        Player* p1;
        Player* p2;
        Player* currentPlayer;
        Player* winner;
        Tile factories[FACTORIES][FACTORY_SIZE];
        LinkedList* pile;
        Player* playerWithFPTile;
        bool saved;
        bool gameEnd;
        bool isTie;
        LinkedList* boxLid;
        LinkedList* tileBag;
        bool resumed;

};

#endif
