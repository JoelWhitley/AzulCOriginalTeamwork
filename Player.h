#ifndef AZUL_PLAYER
#define AZUL_PLAYER

#include "Types.h"
#include <iostream>

class Player {
public:

   Player(std::string name);
   Player(std::string name, int points);
   ~Player();
   std::string getName();
   int getPoints();
   void addPoints(int points);

private:

    std::string name;
    int points;

};

#endif // AZUL_PLAYER