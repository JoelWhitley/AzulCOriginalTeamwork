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
   int countStorage(int row,char tile);
   void setStorage(int row, LinkedList* toInsert);
   void populateStorages();
   void printStorageLine(int row);
   void printMosaicLine(int row);

private:

    std::string name;
    int points;
    char** storage = new char*[5];
    char mosaic[5][5];

};

#endif // AZUL_PLAYER