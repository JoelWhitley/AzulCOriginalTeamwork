#ifndef AZUL_PLAYER
#define AZUL_PLAYER

#include "LinkedList.h"
#include "Types.h"
#include <iostream>


class Player {
public:

   Player(std::string name);
   Player() = default;
   ~Player();
   std::string getName();
   int getPoints();
   void addPoints(int points);
   int countStorage(int row, Tile tile);
   void setStorage(int row, LinkedList* toInsert);
   void populateStorages();
   void printStorageLine(int row);
   void printMosaicLine(int row);
   int calcScore(int row, int col);
   void addToBroken(Tile tile);
   int moveToMosaic(int row,char tile);
   void clearStorageRow(int row);
   Tile getTile(int row);
   LinkedList* getBroken();
   
private:

    std::string name;
    int points;
    Tile** storage = new Tile*[SIZE];
    Tile mosaic[SIZE][SIZE];
    LinkedList* broken;

};

#endif // AZUL_PLAYER