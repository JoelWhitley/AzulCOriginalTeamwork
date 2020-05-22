#ifndef AZUL_PLAYER
#define AZUL_PLAYER

#include "LinkedList.h"
#include "Rules.h"
#include <iostream>
#include <vector>

class Player {

    public:

        Player(std::string name);
        Player() = default;
        ~Player();
        std::string getName();
        void setName(std::string name);
        int getPoints();
        void setPoints(int points);
        void addPoints(int points);
        int countStorage(int row, Tile tile);
        void addToStorage(int row, LinkedList* toInsert, LinkedList* boxLid);
        void addToStorage(int row, std::vector<Tile> toInsert, LinkedList* boxLid);
        void initialiseBoard();
        void printStorageLine(int row);
        void printMosaicLine(int row);
        int calcScore(int row, int col);
        void addToBroken(Tile tile);
        void moveToMosaic(int row, Tile tile);
        void clearStorageRow(int row);
        Tile getTile(int row);
        LinkedList* getBroken();
        bool checkComplete(int row);
        Tile mosaic[SIZE][SIZE]; // Momentarily placed in public for testing
        Tile** storage = new Tile*[SIZE];
        bool mosaicRowHasTile(int row, Tile tile);

    private:
    
        std::string name;
        int points;
        LinkedList* broken;

};

#endif // AZUL_PLAYER
